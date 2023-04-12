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
        igris::dlist_base rx_wait = {};

        pid_t gid = 0; // holder group id
        pid_t pid = 0;

    public:
        tty();
        tty(const char *name, tty_driver *drv, tty_linedisc *ldisc);

        int write(const void *data, unsigned int size) override;
        int read(void *data, unsigned int size) override;

        void receive_newchar(char c);
        void send_signal(int sig);

        int on_open() override;
        int on_release() override;

        size_t send_over_driver(const char *data, size_t size);
        size_t send_over_driver(char c)
        {
            return send_over_driver(&c, 1);
        }
        void notify_readers();
        void signal(int sig);
    };
}

#endif