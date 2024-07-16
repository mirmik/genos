/** @file */

#include <crow/gates/udpgate.h>
#include <crow/tower.h>

#include <fcntl.h>
#include <unistd.h>

#ifdef __WIN32__
//#include <winsock.h>
#include <winsock2.h>
#define SHUT_RDWR 2
#else
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/uio.h>
#endif

#include <memory>
#include <nos/print.h>
#include <nos/util/osutil.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#ifdef CROW_USE_ASYNCIO
#include <crow/asyncio.h>
#endif

void crow::udpgate::read_handler(int fd)
{
    (void)fd; // only one

    crow::header_v1 header;

    struct sockaddr_in sender;
    socklen_t sender_socksize = sizeof(sender);
    memset(&sender, 0, sizeof(sender));

    ssize_t len =
        recvfrom(sock, (char *)&header, sizeof(crow::header_v1), MSG_PEEK,
                 (struct sockaddr *)&sender, &sender_socksize);

    if (len <= 0)
        return;

    if (_debug)
        nos::println("udpgate: recvfrom: ", len);

    crow::packet *block =
        allocate_packet<crow::header_v1>(header.addrsize(), header.datasize());
    block->parse_header(header);

    len = recvfrom(sock, (char *)block->header_addr(), block->fullsize(), 0,
                   (struct sockaddr *)&sender, &sender_socksize);

    if (len <= 0)
    {
        crow::deallocate_packet(block);
        block = nullptr;
        return;
    }

    crow::packet_initialization(block, this);

    nos::buffer vec[3] = {{(char *)&id, 1},
                          {(char *)&sender.sin_addr.s_addr, 4},
                          {(char *)&sender.sin_port, 2}};

    block->revert(vec, 3);

    crow::packet *pack = block;
    block = NULL;

    crow::nocontrol_travel(pack, true);
}

int crow::udpgate::open(uint16_t port)
{
    int ret;

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock < 0)
    {
        perror("udp socket open:");
        printf("code %d", sock);
        exit(0);
    }

    struct sockaddr_in ipaddr;
    socklen_t iplen = sizeof(struct sockaddr_in);
    memset(&ipaddr, 0, iplen);
    ipaddr.sin_port = htons(port);
    ipaddr.sin_family = PF_INET;

    if (port != 0)
    {
        ret = ::bind(sock, (struct sockaddr *)&ipaddr, iplen);

        if (ret != 0)
        {
            perror("crow::udpgate::bind");
            exit(-1);
        }
    }

    else
    {
        // dynamic port assign
        ret = 0;
    }

    nos::osutil::nonblock(sock, true);

#ifdef CROW_USE_ASYNCIO
    crow::asyncio.add_iotask(
        sock, SelectType::READ,
        igris::make_delegate(&udpgate::read_handler, this));
#endif

    return ret;
}

void crow::udpgate::close()
{
    if (sock > 0)
    {
        ::shutdown(sock, SHUT_RDWR);
        ::close(sock);
        sock = -1;
    }
}

void crow::udpgate::send(crow::packet *pack)
{
    uint32_t *addr = (uint32_t *)(pack->stageptr() + 1);
    uint16_t *port = (uint16_t *)(pack->stageptr() + 5);

    struct sockaddr_in ipaddr;
    socklen_t iplen = sizeof(struct sockaddr_in);
    memset(&ipaddr, 0, iplen);

    //Конвертация не требуется, т.к. crow использует сетевой порядок записи
    //адреса.
    ipaddr.sin_port = *port;
    ipaddr.sin_addr.s_addr = *addr;

    header_v1 header = pack->extract_header_v1();

    if (send_buffer.size() < header.flen)
        send_buffer.resize(header.flen);
    memcpy(send_buffer.data(), &header, sizeof(header));
    memcpy(send_buffer.data() + sizeof(header), pack->addrptr(),
           pack->addrsize());
    memcpy(send_buffer.data() + sizeof(header) + pack->addrsize(),
           pack->dataptr(), pack->datasize());

    sendto(sock, (char *)send_buffer.data(), header.flen, 0,
           (struct sockaddr *)&ipaddr, iplen);
    crow::return_to_tower(pack, CROW_SENDED);
}

int crow::create_udpgate(uint8_t id, uint16_t port)
{
    int sts;

    crow::udpgate *g = new crow::udpgate;
    if ((sts = g->open(port)))
        return sts;

    if ((sts = g->bind(id)))
    {
        g->close();
        return sts;
    }

    return 0;
}

std::shared_ptr<crow::udpgate> crow::create_udpgate_safe(uint8_t id,
                                                         uint16_t port)
{
    int sts;

    auto g = std::make_shared<crow::udpgate>();
    if ((sts = g->open(port)))
        return g;

    if ((sts = g->bind(id)))
    {
        g->close();
        return g;
    }

    return g;
}
