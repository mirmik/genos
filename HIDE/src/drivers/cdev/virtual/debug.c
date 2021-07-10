#include <drivers/cdev/virtual/debug.h>
#include <drivers/cdev/cdev.h>

#include <mvfs/file.h>
#include <mvfs/mvfs.h>

#include <stdio.h>
#include <string.h>

#include <igris/dprint.h>

/*static
int open(struct file * f) override
{
	//i->nlink++;
	debug_print_line("debug_open");
	return 0;
}

static
int release (struct file * f) override
{
	//i->nlink--;
	debug_print_line("debug_release");
	return 0;
}*/

static
int write (struct char_device * cdev, const char* data, unsigned int size, int flags)
{
	debug_write(data, size);
	return size;
}

static
int read (struct char_device * cdev, char* data, unsigned int size, int flags)
{
	memset(data, 0, size);
	return size;
}

const struct char_device_operations debug_ops = 
{
	.write = write,
	.read = read
};

CHAR_DEVICE(debug_device, "debug", NULL, &debug_ops);

int link_debug_device(const char* dir)
{
	return vfs_link_cdev(&debug_device, dir, "debug");
}