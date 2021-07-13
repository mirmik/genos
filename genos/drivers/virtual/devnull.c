#include <genos/drivers/virtual/devnull.h>

int devnull_write(struct file_head * file, const void * data, unsigned int size) 
{
	return 0;
}

int devnull_read(struct file_head * file, void * data, unsigned int size) 
{
	return 0;	
}

int devnull_onopen(struct file_head * file) 
{
	return 0;	
}

int devnull_onrelease(struct file_head * file) 
{
	return 0;	
}

const struct file_operations devnull_ops = {
	.write = devnull_write,
	.read = devnull_read,
	.on_open = devnull_onopen,
	.on_release = devnull_onrelease
};

void devnull_init(struct devnull * null) 
{
	device_head_init(&null->dev, "null", &devnull_ops);
}