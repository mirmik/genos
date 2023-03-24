#include <genos/chardev.h>
#include <genos/schedee_api.h>

genos::zillot_chardev::zillot_chardev(zillot::chardev *zchar, const char *name)
    : genos::chardev(name), zchar(zchar)
{
    zchar->rx_callback =
        igris::make_delegate(&zillot_chardev::rx_callback, this);
    zchar->tx_callback =
        igris::make_delegate(&zillot_chardev::tx_callback, this);
}

int genos::zillot_chardev::write(const void *data, unsigned int size)
{
    return zchar->write(data, size);
}

int genos::zillot_chardev::read(void *data, unsigned int size)
{
    if (zchar->avail() == 0)
    {
        wait_current_schedee(&rx_wait, 0, nullptr);
        genos::current_schedee_displace();
    }

    if (data == nullptr)
        return 0;

    return zchar->read(data, size);
}

void genos::zillot_chardev::wait_for_avail()
{
    wait_current_schedee(&rx_wait, 0, nullptr);
}

int genos::zillot_chardev::on_open()
{
    return 0;
}

int genos::zillot_chardev::on_release()
{
    return 0;
}

void genos::zillot_chardev::rx_callback()
{
    unwait_one(&rx_wait, 0);
}

void genos::zillot_chardev::tx_callback()
{
    unwait_one(&tx_wait, 0);
}
