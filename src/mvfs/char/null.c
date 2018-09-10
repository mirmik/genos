#include <mvfs/char/null.h>
#include <mvfs/cdev.h>
#include <mvfs/file.h>
#include <mvfs/mvfs.h>

#include <gxx/panic.h>

#include <stdio.h>

struct file_operations null_f_ops;

//Заполнить struct file.
int null_open(struct inode * i, struct file * f) {}
int null_release (struct inode * i, struct file * f) {}

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
	return mvfs_link_cdev(&null_device, &null_f_ops, dir, "null");	
}