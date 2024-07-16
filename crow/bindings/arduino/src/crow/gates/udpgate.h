/** @file */

#ifndef CROW_GATES_UDPGATE_H
#define CROW_GATES_UDPGATE_H

#include <crow/defs.h>
#include <crow/gateway.h>
#include <iostream>

namespace crow
{

    class udpgate : public gateway
    {
        int sock = 0;
        crow_packet *block = nullptr;
        bool fastsend = CROW_FASTSEND_DEFAULT;

      public:
        udpgate() {}
        udpgate(uint16_t port) { open(port); }

        void send(crow_packet *) override;
        void nblock_onestep() override;

        int open(uint16_t port = 0);
        void close();
        void finish() override;
        void set_fastsend_policy(bool en) { fastsend = en; }

        int bind(int gate_no = CROW_UDPGATE_NO)
        {
            return gateway::bind(gate_no);
        }

#if CROW_ENABLE_WITHOUT_FDS
#else
        int get_fd() override { return sock; }
#endif

        ~udpgate() override { finish(); }
    };

    int create_udpgate(uint8_t id = CROW_UDPGATE_NO, uint16_t port = 0);
} // namespace crow

#endif
