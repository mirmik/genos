#include <mvfs/char/debug.h>
#include <mvfs/cdev.h>
#include <mvfs/file.h>
#include <mvfs/mvfs.h>

#include <gxx/panic.h>

#include <stdio.h>
#include <string.h>

#include <gxx/debug/dprint.h>

struct file_operations debug_f_ops;

//Заполнить struct file.
int __debug_open(struct inode * i, struct file * f) {
	debug_print_line("debug_open");
}

int __debug_release (struct inode * i, struct file * f) {
	debug_print_line("debug_release");
}

int __debug_write (struct file * f, const char* data, unsigned int size) {
	debug_write(data, size);
	return size;
}

int __debug_read (struct file * f, char* data, unsigned int size) {
	memset(data, 0, size);
	return size;
}

struct file_operations debug_f_ops = {
	.open = __debug_open,
	.release = __debug_release,
	.write = __debug_write,
	.read = __debug_read,
};

struct char_device debug_device;

int link_debug_device(const char* dir) {
	return mvfs_link_cdev(&debug_device, &debug_f_ops, dir, "debug");	
}