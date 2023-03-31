#ifndef GENOS_DRIVERS_TTY_H
#define GENOS_DRIVERS_TTY_H

#include <genos/chardev.h>

namespace genos
{
    class tty_driver;
    class tty_linedisc;

    class tty : public genos::chardev
    {
        tty_driver *driver = nullptr;
        tty_linedisc *linedisc = nullptr;

        int gid = 0; // holder group id

    public:
        tty();
        tty(const char *name, tty_driver *drv, tty_linedisc *ldisc);

        int write(const void *data, unsigned int size) override;
        int read(void *data, unsigned int size) override;
        void receive_newchar(char c);
        void send_signal(int sig);
    };
}

#endif