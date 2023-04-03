#ifndef GENOS_CHARDEV_H
#define GENOS_CHARDEV_H

#include <genos/resource/file_head.h>
#include <genos/resource/namespace.h>
#include <igris/container/static_string.h>
#include <igris/datastruct/dlist.h>
#include <igris/osinter/wait.h>
#include <zillot/common/chardev.h>

namespace genos
{
    class chardev : public genos::file_head
    {
        igris::static_string<8> _name;

    public:
        dlist_head chardev_list_lnk = DLIST_HEAD_INIT(chardev_list_lnk);
        int reference_counter = 0;

    public:
        chardev(const char *name);
        ~chardev();
        const char *name()
        {
            return _name.c_str();
        }
    };

    class zillot_chardev : public genos::chardev
    {
        zillot::chardev *zchar = nullptr;
        dlist_head rx_wait = DLIST_HEAD_INIT(rx_wait);
        dlist_head tx_wait = DLIST_HEAD_INIT(tx_wait);

    public:
        zillot_chardev(zillot::chardev *zchar, const char *name);
        int write(const void *data, unsigned int size) override;
        int read(void *data, unsigned int size) override;
        int on_open() override;
        int on_release() override;
        void rx_callback();
        void tx_callback();

        int wait_for_avail() override;
    };

    class device_namespace_manager : public genos::namespace_manager
    {
    public:
        device_namespace_manager() : genos::namespace_manager("/dev") {}
        int lookup(genos::file_head **fh, const char *path) override;
    };
}

#endif