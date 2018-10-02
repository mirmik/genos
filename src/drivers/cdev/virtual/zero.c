#include <drivers/cdev/virtual/zero.h>
#include <drivers/cdev/cdev.h>

#include <mvfs/file.h>
#include <mvfs/mvfs.h>

#include <gxx/panic.h>

#include <stdio.h>
#include <string.h>

struct file_operations zero_f_ops;

//Заполнить struct file.
int zero_open(struct node * i, struct file * f) { return 0; }
int zero_release (struct node * i, struct file * f) { return 0; }

int zero_write (struct file * f, const char* data, unsigned int size) {
	return size;
}

int zero_read (struct file * f, char* data, unsigned int size) {
	memset(data, 0, size);
	return size;
}

struct file_operations zero_f_ops = {
	.open = zero_open,
	.release = zero_release,
	.write = zero_write,
	.read = zero_read,
};

struct char_device zero_device;

int link_zero_device(const char* dir) {
	return vfs_link_cdev(&zero_device, &zero_f_ops, dir, "zero");	
}