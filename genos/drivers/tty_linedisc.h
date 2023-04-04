#ifndef GENOS_DRIVERS_TTY_LINEDISC_H
#define GENOS_DRIVERS_TTY_LINEDISC_H

#include <genos/drivers/tty.h>

namespace genos
{
    class tty_linedisc
    {
    protected:
        genos::tty *_tty = nullptr;

    public:
        virtual void receive_newchar(char c) = 0;
        virtual int transmit(const char *data, size_t size) = 0;
        virtual int read(char *data, size_t size) = 0;
        virtual size_t avail() = 0;
        void attach_tty(genos::tty *tty)
        {
            _tty = tty;
        }

        genos::tty *tty()
        {
            return _tty;
        }
    };
}

#endif