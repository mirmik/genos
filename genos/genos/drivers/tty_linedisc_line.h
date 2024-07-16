#ifndef GENOS_DRIVERS_TTY_LINEDISC_LINE_H
#define GENOS_DRIVERS_TTY_LINEDISC_LINE_H

#include <genos/drivers/tty.h>
#include <genos/drivers/tty_linedisc.h>
#include <igris/container/ring.h>
#include <igris/container/sline.h>
#include <igris/defs/signal.h>
#include <igris/shell/readlinexx.h>

namespace genos
{
    class tty_linedisc_line : public genos::tty_linedisc
    {
        igris::sline sl;
        igris::ring<char> rxring;
        char lastchar = 0;
        bool echo = false;

    public:
        tty_linedisc_line() : sl(48), rxring(256) {}
        tty_linedisc_line(size_t sz) : sl(sz), rxring(256) {}

        size_t avail() override
        {
            return rxring.avail();
        }

        void receive_newchar(char c) override
        {
            if (c == '\n' && lastchar == '\r')
            {
                lastchar = c;
                return;
            }

            if (c == 3)
            {
                if (echo)
                    _tty->send_over_driver("^C\r\n", 4);

                _tty->signal(SIGINT);
                return;
            }

            if (c == '\r' || c == '\n')
            {
                rxring.push('\r');
                rxring.push('\n');
                if (echo)
                    _tty->send_over_driver("\r\n", 2);
                _tty->notify_readers();
            }
            else
            {
                rxring.push(c);
                if (echo)
                    _tty->send_over_driver(&c, 1);
            }

            lastchar = c;
            return;
        }

        int transmit(const char *data, size_t size) override
        {
            return tty()->send_over_driver(data, size);
        }

        int read(char *data, size_t size) override
        {
            return rxring.read(data, size);
        }
    };
}

#endif