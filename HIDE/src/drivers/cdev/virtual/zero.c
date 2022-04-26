#include <drivers/cdev/virtual/zero.h>
#include <drivers/cdev/cdev.h>

#include <mvfs/file.h>
#include <mvfs/mvfs.h>

#include <stdio.h>
#include <string.h>

static
int write (struct char_device * cdev, const char* data, unsigned int size, int flags)
{
	return size;
}

static
int read (struct char_device * cdev, char* data, unsigned int size, int flags)
{
	memset(data, 0, size);
	return size;
}

const struct char_device_operations zero_ops =
{
	.write = write,
	.read = read
};

CHAR_DEVICE(zero_device, "zero", NULL, &zero_ops);

int link_zero_device(const char* dir)
{
	return vfs_link_cdev(&zero_device, dir, "zero");
}