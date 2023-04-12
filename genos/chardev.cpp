#include <asm/irq.h>
#include <genos/chardev.h>
#include <genos/schedee_api.h>

__attribute__((
    init_priority(140))) static igris::dlist<genos::chardev,
                                             &genos::chardev::chardev_list_lnk>
    chardev_list;

genos::device_namespace_manager device_namespace;

int genos::device_namespace_manager::lookup(genos::file_head **fh,
                                            const char *path)
{
    assert(*fh == nullptr);
    for (auto &dev : chardev_list)
    {
        if (strcmp(dev.name(), path) == 0)
        {
            *fh = &dev;
            return 0;
        }
    }
    return -1;
}

genos::chardev::chardev(const char *name) : _name(name)
{
    chardev_list.move_back(*this);
}

genos::chardev::~chardev()
{
    chardev_list.pop(*this);
}

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
    if (data == nullptr)
        return wait_for_avail();

    if (zchar->avail() == 0)
    {
        wait_current_schedee(&rx_wait, 0, nullptr);
        genos::current_schedee_displace();
    }

    if (data == nullptr)
        return 0;

    return zchar->read(data, size);
}

int genos::zillot_chardev::wait_for_avail()
{
    irqstate_t save = irqs_save();
    if (zchar->avail() == 0)
        wait_current_schedee(&rx_wait, 0, nullptr);
    irqs_restore(save);
    return 0;
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
