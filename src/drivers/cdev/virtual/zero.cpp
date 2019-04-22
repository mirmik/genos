#include <drivers/cdev/virtual/zero.h>
#include <drivers/cdev/cdev.h>

#include <mvfs/file.h>
#include <mvfs/mvfs.h>

#include <stdio.h>
#include <string.h>


struct zero_device_class : public char_device {
	int open(struct file * f) override {
		return 0;
	}
	
	int release (struct file * f) override {
		return 0;
	}
	
	int write (const char* data, unsigned int size) override {
		return size;
	}
	
	int read (char* data, unsigned int size) override {
		memset(data, 0, size);
		return size;
	}
} zero_device;

int link_zero_device(const char* dir) {
	return vfs_link_cdev(&zero_device, dir, "zero");	
}