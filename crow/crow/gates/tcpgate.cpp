#include <crow/gates/tcpgate.h>
#include <nos/inet/tcp_client.h>

std::shared_ptr<crow::tcpgate> crow::create_tcpgate_safe(uint8_t id,
                                                         uint16_t port)
{
    int sts;

    auto g = std::make_shared<crow::tcpgate>();
    if ((sts = g->open(port)))
        return g;

    if ((sts = g->bind(id)))
    {
        g->close();
        return g;
    }

    return g;
}

int crow::tcpgate::open(uint16_t port)
{
    server.init();
    int sts = server.bind("0.0.0.0", port);
    if (sts)
        return sts;

    sts = server.listen();
    if (sts)
        return sts;

    server.nonblock(true);

    return sts;
}

void crow::tcpgate::close()
{
    server.close();

    for (auto &s : sockets)
    {
        s.second.close();
    }
}

void crow::tcpgate::send(crow::packet *pack)
{
    uint32_t *addr = (uint32_t *)(pack->stageptr() + 1);
    uint16_t *port = (uint16_t *)(pack->stageptr() + 5);

    nos::inet::tcp_client sock;
    sock.connect(*addr, *port);

    nos::println("connect success");
}
