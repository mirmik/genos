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
	uint8_t refs;
	struct node * node; ///< Родительский node. Держит ссылку.
};

__BEGIN_DECLS

__END_DECLS

#endif