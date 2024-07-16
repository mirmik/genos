#include <fcntl.h>
#include <stdio.h>

#ifdef __WIN32__
#include <winsock2.h>
#include <ws2tcpip.h>
#elif defined(_MSC_VER)
#include <io.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>
#endif

#include <errno.h>

#include <nos/inet/dgramm.h>
#include <nos/inet/hostaddr.h>
#include <nos/inet/tcp_server.h>
#include <nos/inet/tcp_socket.h>

#include <nos/util/osutil.h>

#ifdef __WIN32__
typedef unsigned short sa_family_t;
#define SHUT_RDWR 2
#endif

int nos::inet::socket::nonblock(bool en)
{
    int ret = nos::osutil::nonblock(fd(), en);
    return ret;
}

int nos::inet::socket::nodelay(bool en)
{
    int on = en;
    int rc =
        setsockopt(fd(), IPPROTO_TCP, TCP_NODELAY, (char *)&on, sizeof(on));
    return rc;
}

int nos::inet::socket::dontroute(bool en)
{
    int on = en;
    int rc =
        setsockopt(fd(), SOL_SOCKET, SO_DONTROUTE, (char *)&on, sizeof(on));
    return rc;
}

int nos::inet::socket::reusing(bool en)
{
    int on = en;
    int rc =
        setsockopt(fd(), SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
    return rc;
}

int nos::inet::socket::init(int domain, int type, int proto)
{
    int64_t fd = ::socket(domain, type, proto);
    if (fd < 0)
    {
        perror("socket::init");
        throw std::runtime_error("socket::init");
    }
    set_fd(fd);
    return (int)this->fd();
}

int nos::inet::socket::bind(const nos::inet::hostaddr &haddr,
                            uint16_t port,
                            int family)
{
    int sts;
    struct sockaddr_in addr;

    memset(&addr, 0, sizeof(addr));

    addr.sin_family = (int)family;
    addr.sin_addr.s_addr = htonl(haddr.addr); // INADDR_ANY = 0.0.0.0
    addr.sin_port = htons(port);

    sts = ::bind(fd(), (sockaddr *)&addr, sizeof(struct sockaddr_in));
    return sts;
}

int nos::inet::socket::listen(int conn)
{
    int sts;

    sts = ::listen(fd(), conn);
    return sts;
}

int nos::inet::socket::connect(const nos::inet::hostaddr &haddr,
                               uint16_t port,
                               int family)
{
    int sts;
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));

    addr.sin_family = (int)family;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(haddr.addr);

    sts = ::connect(fd(), (struct sockaddr *)&addr, sizeof(addr));
    return sts;
}

int nos::inet::socket::close_socket()
{
    int sts = 0;
#ifdef _MSC_VER
    sts = ::shutdown((int)fd(), SD_BOTH);
    sts = ::closesocket((int)fd());
#else
    sts = ::shutdown((int)fd(), SHUT_RDWR);
    sts = nos::osutil::close((int)fd());
#endif
    set_fd(-1);
    return sts;
}

bool nos::inet::socket::is_closed()
{
    return (int)fd() < 0;
}

nos::inet::datagramm_socket::datagramm_socket(int domain, int type, int proto)
{
    socket::init(domain, type, proto);
}

int nos::inet::datagramm_socket::sendto(nos::inet::hostaddr haddr,
                                        uint16_t port,
                                        const char *data,
                                        size_t size)
{
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = PF_INET;
    addr.sin_addr.s_addr = htonl(haddr.addr); // INADDR_ANY = 0.0.0.0
    addr.sin_port = htons(port);
    return ::sendto(
        (int)fd(), data, (int)size, 0, (sockaddr *)&addr, sizeof(sockaddr_in));
}

int nos::inet::datagramm_socket::ne_sendto(uint32_t ipaddr,
                                           uint16_t port,
                                           const char *data,
                                           size_t size)
{
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = PF_INET;
    addr.sin_addr.s_addr = ipaddr; // INADDR_ANY = 0.0.0.0
    addr.sin_port = port;
    return ::sendto(
        (int)fd(), data, (int)size, 0, (sockaddr *)&addr, sizeof(sockaddr_in));
}

int nos::inet::datagramm_socket::recvfrom(char *data,
                                          size_t maxsize,
                                          nos::inet::netaddr *inaddr)
{
    struct sockaddr_in si_other;
    socklen_t sz = sizeof(sockaddr_in);
    int ret = ::recvfrom(
        (int)fd(), data, (int)maxsize, 0, (sockaddr *)&si_other, &sz);

    if (ret < 0)
    {
        return ret;
    }

    if (inaddr)
        *inaddr = nos::inet::netaddr(ntohl(si_other.sin_addr.s_addr),
                                     ntohs(si_other.sin_port));
    return ret;
}

nos::inet::udp_socket::udp_socket()
    : datagramm_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)
{
}

nos::inet::udp_socket::udp_socket(nos::inet::hostaddr addr, uint16_t port)
    : udp_socket()
{
    bind(addr, port);
}

int nos::inet::udp_socket::bind(nos::inet::hostaddr addr, uint16_t port)
{
    return socket::bind(addr, port, PF_INET);
}

nos::inet::rdm_socket::rdm_socket() : datagramm_socket(AF_INET, SOCK_RDM, 0) {}

nos::inet::rdm_socket::rdm_socket(nos::inet::hostaddr addr, uint16_t port)
    : rdm_socket()
{
    socket::bind(addr, port, PF_INET);
}
