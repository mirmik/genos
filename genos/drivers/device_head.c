#include <genos/drivers/device_head.h>
#include <string.h>

DLIST_HEAD(devices_list);

void device_head_init(struct device_head *dev,
                      const char *name,
                      const struct file_operations *f_ops)
{
    strncpy((char *)dev->name, name, DEVICE_NAME_MAXLEN);
    file_head_init(&dev->fil, f_ops);
    dlist_add(&dev->lnk_devices_list, &devices_list);
}
