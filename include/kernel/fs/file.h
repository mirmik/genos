#ifndef KERNEL_FS_FILE_H
#define KERNEL_FS_FILE_H

#include <inttypes.h>

struct file;

typedef int(*kernel_write_t)(struct file*, struct query_t* query);
typedef int(*kernel_read_t)(struct file*, struct query_t* query);

typedef int(*write_t)(struct file*, const void*, size_t);
typedef int(*read_t)(struct file*, const void*, size_t);

typedef int(*avail_t)(struct file*);
typedef int(*room_t)(struct file*);
typedef int(*flush_t)(struct file*);

struct file_operations {
	write_t kernel_write;
	read_t kernel_read;

	write_t write;
	read_t read;

	avail_t avail;
	room_t room;
	flush_t flush;
}

struct file {
	struct file_operations * fops;
}

#endif