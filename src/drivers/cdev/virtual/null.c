#include <drivers/cdev/virtual/null.h>
#include <drivers/cdev/cdev.h>

#include <mvfs/file.h>
#include <mvfs/mvfs.h>

#include <gxx/panic.h>

#include <stdio.h>

struct file_operations null_f_ops;

//Заполнить struct file.
int null_open(struct node * i, struct file * f) { return 0; }
int null_release (struct node * i, struct file * f) { return 0; }

int null_write (struct file * f, const char* data, unsigned int size) {
	return size;
}

int null_read (struct file * f, char* data, unsigned int size) {
	return 0;
}

struct file_operations null_f_ops = {
	.open = null_open,
	.release = null_release,
	.write = null_write,
	.read = null_read,
};

struct char_device null_device;

int link_null_device(const char* dir) {
	return vfs_link_cdev(&null_device, &null_f_ops, dir, "null");	
}