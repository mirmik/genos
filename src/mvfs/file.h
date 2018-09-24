#ifndef MVFS_FILE_H
#define MVFS_FILE_H

/**
 *	@file 
 *	Описатель открытого файла.
 *	Является интерфейсом к данным описываемым inode.
 */

#include <mvfs/super.h>
#include <mvfs/file.h>
#include <errno.h>

struct file_operations;

struct file {
	int pos;
	struct node * f_node; ///< Родительский node. Держит ссылку.
	const struct file_operations* f_op;
};

struct file_operations {
	int (*open) (struct node *, struct file *); ///< заполнить структуру файла по inode.
	int (*release) (struct node *, struct file *);	
	int (*write) (struct file*, const char* data, unsigned int size);
	int (*read) (struct file*, char* data, unsigned int size);

	int (*iterate) (struct file*, void*);
};

__BEGIN_DECLS


__END_DECLS

#endif