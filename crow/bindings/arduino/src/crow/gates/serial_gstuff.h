/**
    @file
*/

#ifndef CROW_GATES_SERIAL_GSTUFF_H
#define CROW_GATES_SERIAL_GSTUFF_H

#include <stdbool.h>
#include <stdint.h>
#include <sys/cdefs.h>

#include <crow/gateway.h>

//#include <igris/protocols/gstuff/autorecv.h>
#include <igris/protocols/gstuff.h>

namespace crow
{
    struct serial_gstuff : public crow::gateway
    {
        int fd;

        struct crow_packet *rpack;
        bool debug;

        struct gstuff_autorecv recver;

        void newline_handler();

        void send(crow_packet *) override;
        void nblock_onestep() override;
        void finish() override {}

#if CROW_ENABLE_WITHOUT_FDS
#else
        virtual int get_fd() { return fd; }
#endif
    };

    crow::serial_gstuff *create_serial_gstuff(const char *path,
                                              uint32_t baudrate, uint8_t id,
                                              bool debug);
} // namespace crow

#endif
