#include <drivers/cdev/pipe.h>

int pipe_write(struct char_device * cdev,
               const char* data,
               unsigned int size,
               int flags);

int pipe_read(struct char_device * cdev,
               char* data,
               unsigned int size,
               int flags);

int pipe_waitread(struct char_device * cdev);

const struct char_device_operations pipe_ops = 
{
	.write = pipe_write,
	.read = pipe_read,
	.waitread = pipe_waitread
};