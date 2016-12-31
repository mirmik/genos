#ifndef GENOS_FILE_H
#define GENOS_FILE_H

#include "inttypes.h"
#include "compiler.h"

struct inode;
struct path;

enum whence {
	SEEK_SET, SEEK_CUR, SEEK_END
}

struct file_operations 
{
	struct file *(*open)(struct inode *node, int flag);
	int    (*close)(struct file *desc);
	size_t (*read)(struct file *desc, void *buf, size_t size, int flag);
	size_t (*write)(struct file *desc, void *buf, size_t size, int flag);
	int    (*ioctl)(struct file *desc, int request, void *data, int flag);
	int    (*lseek)(struct file *desc, off_t off, int flag);
};

struct file {
	struct path 					f_path;
	struct inode *					f_inode;
	atomic_t						f_refs;
	off_t							f_pos;

	const struct file_operations* 	f_ops;
};

#endif