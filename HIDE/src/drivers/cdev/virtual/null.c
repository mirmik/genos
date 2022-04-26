#include <drivers/cdev/virtual/null.h>
#include <drivers/cdev/cdev.h>

#include <mvfs/file.h>
#include <mvfs/mvfs.h>

#include <stdio.h>
#include <mvfs/trace.h>

static
int write (struct char_device * cdev, const char* data, unsigned int size, int flags)
{
	DTRACE();
	return size;
}

static
int read (struct char_device * cdev, char* data, unsigned int size, int flags)
{
	DTRACE();
	return 0;
}

const struct char_device_operations null_ops =
{
	.write = write,
	.read = read
};

CHAR_DEVICE(null_device, "null", NULL, &null_ops);

int link_null_device(const char* dir)
{
	DTRACE();
	return vfs_link_cdev(&null_device, dir, "null");
}