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
	struct inode * f_inode; ///< Родительский inode. Держит ссылку.
	const struct file_operations* f_op;
};

struct file_operations {
	int (*open) (struct inode *, struct file *); ///< заполнить структуру файла по inode.
	int (*release) (struct inode *, struct file *);	
	int (*write) (struct file*, const char* data, unsigned int size);
	int (*read) (struct file*, char* data, unsigned int size);
};

__BEGIN_DECLS

struct file* mvfs_file_alloc();
void mvfs_file_destroy(struct file*);

static inline int mvfs_write(struct file * filp, const char* data, unsigned int size) {
	if (filp->f_op->write == NULL) return -ENOTSUP;
	return filp->f_op->write(filp, data, size);
}

static inline int mvfs_read(struct file * filp, char* data, unsigned int size) {
	if (filp->f_op->read == NULL) return -ENOTSUP;
	return filp->f_op->read(filp, data, size);
}

extern int mvfs_open(const char* path, int flags, struct file** filpp);
extern int mvfs_close(struct file* filpp);
extern int mvfs_open_with_root(const char* path, int flags, 
	struct file** filpp, struct dentry * root);

__END_DECLS

#endif