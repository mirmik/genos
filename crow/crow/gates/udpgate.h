/** @file */

#ifndef CROW_GATES_UDPGATE_H
#define CROW_GATES_UDPGATE_H

#include <crow/defs.h>
#include <crow/gateway.h>
#include <igris/container/unbounded_array.h>
#include <memory>

namespace crow
{

    class udpgate : public gateway
    {
        int sock = 0;
        igris::unbounded_array<uint8_t> receive_buffer = {};
        igris::unbounded_array<uint8_t> send_buffer = {};
        bool _debug = false;

    public:
        udpgate() {}
        udpgate(uint16_t port)
        {
            open(port);
        }

        void send(crow::packet *) override;
        void read_handler(int fd);

        bool opened()
        {
            return sock > 0;
        }

        int open(uint16_t port = 0);
        void close();
        void finish() override
        {
            close();
        }

        int bind(int gate_no = CROW_UDPGATE_NO)
        {
            return gateway::bind(gate_no);
        }

        ~udpgate() override
        {
            close();
        }

        void debug(bool val)
        {
            _debug = val;
        }
    };

    int create_udpgate(uint8_t id = CROW_UDPGATE_NO, uint16_t port = 0);
    std::shared_ptr<crow::udpgate> create_udpgate_safe(uint8_t id,
                                                       uint16_t port = 0);
}

#endif
