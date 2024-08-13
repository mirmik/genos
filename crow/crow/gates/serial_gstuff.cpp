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

crow::serial_gstuff::serial_gstuff(gstuff_context gctx)
    : gctx(gctx), recver(gctx)
{
}

void crow::serial_gstuff::newline_handler()
{
    crow::packet *block = rpack;
    rpack = NULL;

    block->revert_gate(id);

    crow::packet_initialization(block, this);
    crow::nocontrol_travel(block, false);
}

void crow::serial_gstuff::send(crow::packet *pack)
{
    char buffer[512];

    header_v1 header = pack->extract_header_v1();
    struct iovec iov[] = {
        {&header, sizeof(header)},
        {pack->addrptr(), pack->addrsize()},
        {pack->dataptr(), pack->datasize()},
    };
    int size = gstuffing_v(iov, 3, buffer, gctx);
    auto _ = write(fd, buffer, size);
    (void)_;
    crow::return_to_tower(pack, CROW_SENDED);
}

void crow::serial_gstuff::read_handler(int)
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
            rpack = allocate_packet<crow::header_v1>(GSTUFF_MAXPACK_SIZE);
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

void crow::serial_gstuff::setup_serial_port(int baud,
                                            char parity,
                                            int databits,
                                            int stopbits)
{
    nos::serial_port port(fd);
    port.setup(baud, parity, databits, stopbits);
}

void crow::serial_gstuff::finish() {}

// template <class Header>
crow::serial_gstuff *crow::create_serial_gstuff(const char *path,
                                                uint32_t baudrate,
                                                uint8_t id,
                                                bool debug,
                                                const gstuff_context& gctx)
{
    (void)baudrate;
    auto *g = new crow::serial_gstuff(gctx);

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
        g->fd,
        SelectType::READ,
        igris::make_delegate(&serial_gstuff::read_handler, g));

    return g;
}















crow::serial_gstuff_v0::serial_gstuff_v0(gstuff_context gctx)
    : gctx(gctx), recver(gctx)
{
}

void crow::serial_gstuff_v0::newline_handler()
{
    crow::packet *block = rpack;
    rpack = NULL;

    block->revert_gate(id);

    crow::packet_initialization(block, this);
    crow::nocontrol_travel(block, false);
}

void crow::serial_gstuff_v0::send(crow::packet *pack)
{
    char buffer[512];

    header_v0 header = pack->extract_header_v0();
    struct iovec iov[] = {
        {&header, sizeof(header)},
        {pack->addrptr(), pack->addrsize()},
        {pack->dataptr(), pack->datasize()},
    };
    int size = gstuffing_v(iov, 3, buffer, gctx);
    auto _ = write(fd, buffer, size);
    (void)_;
    crow::return_to_tower(pack, CROW_SENDED);
}

void crow::serial_gstuff_v0::read_handler(int)
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
            rpack = allocate_packet<crow::header_v0>(GSTUFF_MAXPACK_SIZE);
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

void crow::serial_gstuff_v0::setup_serial_port(int baud,
                                            char parity,
                                            int databits,
                                            int stopbits)
{
    nos::serial_port port(fd);
    port.setup(baud, parity, databits, stopbits);
}

void crow::serial_gstuff_v0::finish() {}

// template <class Header>
crow::serial_gstuff_v0 *crow::create_serial_gstuff_v0(const char *path,
                                                uint32_t baudrate,
                                                uint8_t id,
                                                bool debug,
                                                const gstuff_context& gctx)
{
    (void)baudrate;
    auto *g = new crow::serial_gstuff_v0(gctx);

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
        g->fd,
        SelectType::READ,
        igris::make_delegate(&serial_gstuff_v0::read_handler, g));

    return g;
}
