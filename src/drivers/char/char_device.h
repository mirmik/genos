#ifndef GENOS_DRIVERS_CHAR_DEVICE_H
#define GENOS_DRIVERS_CHAR_DEVICE_H

#include <gxx/datastruct/dlist.h>
#include <fs/file.h>

extern struct dlist_head cdev_list;

/*Использовать как заголовок*/
struct char_device {
	struct dlist_head cdev_list;
	const struct file_operations *fops;
};

__BEGIN_DECLS

int char_device_register(struct char_device *cdev, const char* name, const struct file_operations* ops);

__END_DECLS

#endif