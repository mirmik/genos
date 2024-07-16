#ifndef CROW_GATES_TCPGATE_H
#define CROW_GATES_TCPGATE_H

#include <crow/gateway.h>
#include <map>
#include <memory>
#include <nos/inet/tcp_client.h>
#include <nos/inet/tcp_server.h>

#define CROW_TCPGATE_NO 13

namespace crow
{
    class tcpgate : public gateway
    {
        nos::inet::tcp_server server = {};
        std::map<nos::inet::netaddr, nos::inet::tcp_client> sockets = {};

    public:
        tcpgate() {}
        tcpgate(uint16_t port)
        {
            open(port);
        }

        void send(crow::packet *) override;
        bool opened()
        {
            return server.fd() > 0;
        }

        int open(uint16_t port = 0);
        void close();
        void finish() override
        {
            close();
        }

        int bind(int gate_no = CROW_TCPGATE_NO)
        {
            return gateway::bind(gate_no);
        }

        void open_new_client(nos::inet::netaddr addr)
        {
            auto sock = nos::inet::tcp_client(addr);
            sockets[addr] = sock;
        }

        ~tcpgate() override
        {
            close();
        }
    };

    std::shared_ptr<crow::tcpgate> create_tcpgate_safe(uint8_t id,
                                                       uint16_t port = 0);
}

#endif