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
    template <class Header> struct serial_gstuff : public crow::gateway
    {
        int fd = -1;

        crow::packet *rpack = nullptr;
        bool debug = false;
        gstuff_autorecv recver = {};

    public:
        void newline_handler()
        {
            crow::packet *block = rpack;
            rpack = NULL;

            block->revert_gate(id);

            crow::packet_initialization(block, this);
            crow::nocontrol_travel(block, false);
        }

        void send(crow::packet *pack) override
        {
            char buffer[512];

            header_v1 header = pack->extract_header_v1();
            struct iovec iov[] = {
                {&header, sizeof(header)},
                {pack->addrptr(), pack->addrsize()},
                {pack->dataptr(), pack->datasize()},
            };
            int size = gstuffing_v(iov, 3, buffer);
            auto _ = write(fd, buffer, size);
            (void)_;
            crow::return_to_tower(pack, CROW_SENDED);
        }

        void read_handler(int)
        {
#define GSTUFF_MAXPACK_SIZE 512
            char buf[1024];
            ssize_t len = read(fd, (uint8_t *)buf, 1024);

            for (int i = 0; i < len; ++i)
            {
                char c = buf[i];

                if (debug)
                {
                    debug_printhex_uint8(c);
                    debug_putchar('\t');
                    debug_putchar(c);
                    debug_print_newline();
                }

                if (rpack == NULL)
                {
                    rpack =
                        allocate_packet<crow::header_v1>(GSTUFF_MAXPACK_SIZE);
                    recver.setbuf(rpack->header_addr(), GSTUFF_MAXPACK_SIZE);
                }

                int ret = recver.newchar(c);

                switch (ret)
                {
                    case GSTUFF_CRC_ERROR:
                        crow::warn("warn: gstuff crc error");
                        break;

                    case GSTUFF_NEWPACKAGE:
                        newline_handler();
                        break;

                    default:
                        break;
                }
            }
        }

        void
        setup_serial_port(int baud, char parity, int databits, int stopbits)
        {
            nos::serial_port port(fd);
            port.setup(baud, parity, databits, stopbits);
        }

        void finish() override {}
    };

    template <class Header>
    crow::serial_gstuff<Header> *create_serial_gstuff(const char *path,
                                                      uint32_t baudrate,
                                                      uint8_t id,
                                                      bool debug)
    {
        (void)baudrate;
        auto *g = new crow::serial_gstuff<Header>;

        g->debug = debug;

        g->fd = open(path, O_RDWR | O_NOCTTY);
        fcntl(g->fd, F_SETFL, fcntl(g->fd, F_GETFL) | O_NONBLOCK);

        if (g->fd < 0)
        {
            perror("serial::open");
            exit(0);
        }

        g->rpack = NULL;
        g->bind(id);

        crow::asyncio.add_iotask(
            g->fd, SelectType::READ,
            igris::make_delegate(&serial_gstuff<Header>::read_handler, g));

        return g;
    }

} // namespace crow

#endif
