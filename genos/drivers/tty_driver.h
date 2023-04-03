#ifndef GENOS_DRIVERS_TTY_DRIVER_H
#define GENOS_DRIVERS_TTY_DRIVER_H

#include <genos/drivers/tty.h>

namespace genos
{
    class tty_driver
    {
    protected:
        tty *_tty = nullptr;

    public:
        virtual void transmit(const char *data, size_t size) = 0;
        void set_tty(genos::tty *tty)
        {
            _tty = tty;
        }

        void receive_newchar(char c)
        {
            _tty->receive_newchar(c);
        }

        virtual ~tty_driver() = default;
    };
}

#endif