#ifndef GENOS_MVFS_MVFS_H
#define GENOS_MVFS_MVFS_H

/**
 *	Попытка написать минимальный vfs.
 *
 */

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include <mvfs/compat.h>
#include <mvfs/pathops.h>

#include <mvfs/fstype.h>
#include <mvfs/dentry.h>
#include <mvfs/super.h>

__BEGIN_DECLS

///Инициализация структур данных vfs.
extern void vfs_init();

///Провести процедуру lookup от current.
extern int vfs_lookup_relative(const char* str_path, const char** pend, struct dentry** current);

///Провести процедуру lookup по относительному пути.
static inline int vfs_lookup(const char* str_path, const char** pend, struct dentry** current) {
	if (path_is_abs(str_path)) {
		*current = vfs_get_root();
	} else {
		*current = vfs_get_pwd();
	}
	return vfs_lookup_relative(str_path, pend, current); 	
}

///Стандартные функции для реализации методов fs
extern struct dentry * vfs_virtual_lookup(struct inode *, struct dentry *);


/**
 *	@dir parent inode
 *	@d dentry for created inode
 *	@flags
 */
extern int vfs_mkdir_do(struct inode* dir, struct dentry* d, int flags);

/**
 *	@dir deleted inode
 *	@d dentry of deleted inode
 */
extern int vfs_rmdir_do(struct inode* dir, struct dentry* d);

extern int vfs_write(struct file * filp, const char* data, unsigned int size);
extern int vfs_read(struct file * filp, char* data, unsigned int size);
extern int vfs_open(const char* path, int flags, struct file** filpp);
extern int vfs_close(struct file* filpp);


extern int vfs_pwd(char* buf);


extern void vfs_dpr_pwd();

__END_DECLS

#endif