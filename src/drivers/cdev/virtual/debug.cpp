#include <drivers/cdev/virtual/debug.h>
#include <drivers/cdev/cdev.h>

#include <mvfs/file.h>
#include <mvfs/mvfs.h>

#include <gxx/panic.h>

#include <stdio.h>
#include <string.h>

#include <gxx/debug/dprint.h>

struct debug_device_class : public char_device {
	int open(struct file * f) override {
		//i->nlink++;
		debug_print_line("debug_open");
		return 0;
	}
	
	int release (struct file * f) override {
		//i->nlink--;
		debug_print_line("debug_release");
		return 0;
	}
	
	int write (struct file * f, const char* data, unsigned int size) override {
		debug_write(data, size);
		return size;
	}
	
	int read (struct file * f, char* data, unsigned int size) override {
		memset(data, 0, size);
		return size;
	}
} debug_device;

int link_debug_device(const char* dir) {
	return vfs_link_cdev(&debug_device, dir, "debug");	
}