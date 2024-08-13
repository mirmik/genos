/**
    @file
*/

#ifndef CROW_GATES_SERIAL_GSTUFF_V1_H
#define CROW_GATES_SERIAL_GSTUFF_V1_H

#include <stdbool.h>
#include <stdint.h>
#include <sys/cdefs.h>

#include <crow/gateway.h>

#include <igris/protocols/gstuff_v1/autorecv.h>
#include <igris/protocols/gstuff_v1/gstuff.h>

namespace crow
{
    struct serial_gstuff_v1 : public crow::gateway
    {
        int fd;

        struct crow_packet *rpack;
        bool debug;

        struct gstuff_autorecv_v1 recver;

        void newline_handler();

        void send(crow_packet *) override;
        void nblock_onestep() override;
        void finish() override {}

#if CROW_ENABLE_WITHOUT_FDS
#else
        virtual int get_fd() override { return fd; }
#endif
    };

    // crow::serial_gstuff_v1 *create_serial_gstuff_v1(const char *path,
    //                                                 uint32_t baudrate,
    //                                                 uint8_t id, bool debug);
} // namespace crow

#endif
