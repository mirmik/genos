#ifndef GENOS_DRIVERS_TTY_LINEDISC_LINE_H
#define GENOS_DRIVERS_TTY_LINEDISC_LINE_H

#include <genos/drivers/tty.h>
#include <genos/drivers/tty_linedisc.h>
#include <igris/container/ring.h>

namespace genos
{
    class tty_linedisc_line : public genos::tty_linedisc
    {
        igris::ring<char> rxring;

    public:
        tty_linedisc_line() : rxring(48) {}
        tty_linedisc_line(size_t sz) : rxring(sz) {}

        void receive_newchar(char c) override
        {
            rxring.push(c);
            return;
        }

        int transmit(const char *data, size_t size) override
        {
            return 0;
        }

        int read(char *data, size_t size) override
        {
            return rxring.read(data, size);
        }
    };
}

#endif