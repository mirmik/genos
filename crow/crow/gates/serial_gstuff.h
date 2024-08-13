/**
    @file
*/

#ifndef CROW_GATES_SERIAL_GSTUFF_H
#define CROW_GATES_SERIAL_GSTUFF_H

#include <crow/asyncio.h>
#include <crow/gates/serial_gstuff.h>
#include <crow/gateway.h>
#include <crow/tower.h>
#include <crow/warn.h>
#include <fcntl.h>
#include <igris/protocols/gstuff.h>
#include <igris/util/bug.h>
#include <nos/io/serial_port.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/cdefs.h>
#include <termios.h>
#include <unistd.h>

namespace crow
{
    struct serial_gstuff_v0 : public crow::gateway
    {
        int fd = -1;

        gstuff_context gctx;
        crow::packet *rpack = nullptr;
        bool debug = false;
        gstuff_autorecv recver;

    public:
        serial_gstuff_v0(const serial_gstuff_v0 &) = delete;
        serial_gstuff_v0(gstuff_context gctx);
        serial_gstuff_v0 &operator=(const serial_gstuff_v0 &) = delete;

        void newline_handler();

        void send(crow::packet *pack) override;

        void read_handler(int);

        void
        setup_serial_port(int baud, char parity, int databits, int stopbits);

        void finish() override;
    };

    // template <class Header>
    crow::serial_gstuff_v0 *create_serial_gstuff_v0(const char *path,
                                              uint32_t baudrate,
                                              uint8_t id,
                                              bool debug,
                                                const gstuff_context& gctx);


    struct serial_gstuff : public crow::gateway
    {
        int fd = -1;

        gstuff_context gctx;
        crow::packet *rpack = nullptr;
        bool debug = false;
        gstuff_autorecv recver;

    public:
        serial_gstuff(const serial_gstuff &) = delete;
        serial_gstuff(gstuff_context gctx);
        serial_gstuff &operator=(const serial_gstuff &) = delete;

        void newline_handler();

        void send(crow::packet *pack) override;

        void read_handler(int);

        void
        setup_serial_port(int baud, char parity, int databits, int stopbits);

        void finish() override;
    };

    // template <class Header>
    crow::serial_gstuff *create_serial_gstuff(const char *path,
                                              uint32_t baudrate,
                                              uint8_t id,
                                              bool debug,
                                                const gstuff_context& gctx);

} // namespace crow

#endif
