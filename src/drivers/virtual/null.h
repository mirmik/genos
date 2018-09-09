#ifndef GENOS_DRIVERS_NULL_DEVICE_H
#define GENOS_DRIVERS_NULL_DEVICE_H

#include <drivers/char/char_device.h>

int null_open(struct inode *inode, struct file *filp) {

}

int null_release(struct inode *inode, struct file *filp) {

}

int null_write(struct file *file, const char *data, unsigned int size) {

}

int null_read(struct file *file, char *data, unsigned int size) {

}

const struct file_operations null_ops = {
	.read = null_read,
	.write = null_write,
	.open = null_open,
	.release = null_release,
};

struct char_device null_device;

static inline int register_null_device() {
	return char_device_register(&null_device, "null", &null_ops);
}

#endif