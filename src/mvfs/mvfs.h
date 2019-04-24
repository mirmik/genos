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
#include <mvfs/dirent.h>
#include <mvfs/super.h>

__BEGIN_DECLS

///Инициализация структур данных vfs.
extern void vfs_init();

///Провести процедуру lookup от current.
extern int vfs_lookup_relative(const char* str_path, const char** pend, struct node** current);

///Провести процедуру lookup по относительному пути.
static inline int vfs_lookup(const char* str_path, const char** pend, struct node** current) {
	if (path_is_abs(str_path)) {
		*current = vfs_get_root();
	} else {
		*current = vfs_get_pwd();
	}
	return vfs_lookup_relative(str_path, pend, current); 	
}

///Стандартные функции для реализации методов fs
extern struct node * vfs_virtual_lookup(struct node * dir, const char * name, size_t nlen);
extern int vfs_common_open (struct node *, struct file *);
extern int vfs_common_release (struct node *, struct file *);	

/**
 *	@dir parent node
 *	@d dentry for created node
 *	@flags
 */
extern int vfs_mkdir_do(struct node* dir, const char * name, size_t nlen, int flags);

/**
 *	@dir deleted node
 *	@d dentry of deleted node
 */
extern int vfs_rmdir_do(struct node* dir);

extern int vfs_write(struct file * filp, const char* data, unsigned int size, int flags);
extern int vfs_read(struct file * filp, char* data, unsigned int size, int flags);
extern int vfs_open(const char* path, int flags, struct file** filpp);
extern int vfs_close(struct file* filpp);


extern int vfs_pwd(char* buf);


extern void vfs_debug_pwd();

extern int vfs_iterate(struct file * filp, struct dirent * de);
extern int vfs_debug_ls(const char* path);
extern int vfs_debug_tree(const char* path);

__END_DECLS

#endif