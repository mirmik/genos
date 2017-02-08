#include <kernel/fs/chardev.h>

int chardev_write(struct file_s* filp, const void* data, size_t sz) {
	chardev_t * dev = (chardev_t *) filp;
	return dev->cdops->write(dev, data, sz);
}

int chardev_read(struct file_s* filp, const void* data, size_t sz) {
	chardev_t * dev = (chardev_t *) filp;
	return dev->cdops->read(dev, data, sz);	
}

const struct file_operations chardev_file_ops = {
	.write = chardev_write,
	.read = chardev_read,
};