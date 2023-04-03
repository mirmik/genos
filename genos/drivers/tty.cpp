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

int genos::tty::on_open()
{
    auto current_pid = genos::current_schedee()->pid;
    bool is_opened = pid != 0;

    if (is_opened && pid != current_pid)
    {
        return -1;
    }

    pid = current_pid;
    gid = genos::current_schedee()->gid;
    reference_counter++;

    return 0;
}

int genos::tty::on_release()
{
    reference_counter--;
    if (reference_counter == 0)
    {
        pid = 0;
        gid = 0;
        return 0;
    }
    return 0;
}