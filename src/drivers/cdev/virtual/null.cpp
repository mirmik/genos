#include <drivers/cdev/virtual/null.h>
#include <drivers/cdev/cdev.h>

#include <mvfs/file.h>
#include <mvfs/mvfs.h>

#include <gxx/panic.h>

#include <stdio.h>

struct null_device_class : public char_device {
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
		return 0;
	}
} null_device;

int link_null_device(const char* dir) {
	return vfs_link_cdev(&null_device, dir, "null");	
}