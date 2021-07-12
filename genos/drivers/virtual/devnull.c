#include <genos/drivers/virtual/devnull.h>

int devnull_write(struct file_head * file, const void * data, int size) 
{

}

int devnull_read(struct file_head * file, void * data, int size) 
{
	
}

int devnull_onopen(struct file_head * file) 
{
	
}

int devnull_onrelease(struct file_head * file) 
{
	
}

const struct file_operations devnull_ops = {
	.write = devnull_write,
	.read = devnull_read,
	.onopen = devnull_onopen,
	.onrelease = devnull_onrelease
};

void devnull_init(struct devnull * null) 
{
	char_device_init(devnull, "null", &devnull_ops);
}