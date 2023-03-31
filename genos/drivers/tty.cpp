#include <genos/drivers/tty.h>
#include <genos/drivers/tty_driver.h>
#include <genos/drivers/tty_linedisc.h>
#include <genos/schedee_api.h>

genos::tty::tty(const char *name,
                genos::tty_driver *drv,
                genos::tty_linedisc *ldisc)
    : chardev(name), driver(drv), linedisc(ldisc)
{
}

int genos::tty::write(const void *data, unsigned int size)
{
    return linedisc->transmit((const char *)data, size);
}

int genos::tty::read(void *data, unsigned int size)
{
    return linedisc->read((char *)data, size);
}

void genos::tty::receive_newchar(char c)
{
    linedisc->receive_newchar(c);
}

void genos::tty::send_signal(int sig)
{
    genos::send_signal_to_group(gid, sig);
}