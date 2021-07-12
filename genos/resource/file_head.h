#ifndef GENOS_RESOURCE_FILE_HEAD_H
#define GENOS_RESOURCE_FILE_HEAD_H

#include <igris/compiler.h>
#include <stdint.h>

struct file_head;

struct file_operations 
{
	int (*write)(struct file_head * file, const void * data, int size);
	int (*read) (struct file_head * file,       void * read, int size);

	int (*on_open)    (struct file_head * file);
	int (*on_release) (struct file_head * file);
};

struct file_head 
{
	const struct file_operations * f_ops;
	uint8_t refs;
};

__BEGIN_DECLS

void file_head_init(struct file_head * file, const struct file_operations * ops);
int  file_head_is_open(struct file_head * file);

__END_DECLS

#endif