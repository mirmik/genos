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

///Инициализация структур данных mvfs.
extern void mvfs_init();

///Провести процедуру lookup от current.
extern int mvfs_lookup_relative(const char* str_path, const char** pend, struct dentry** current);

///Провести процедуру lookup по относительному пути.
static inline int mvfs_lookup(const char* str_path, const char** pend, struct dentry** current) {
	if (path_is_abs(str_path)) {
		*current = mvfs_get_root();
	} else {
		*current = mvfs_get_pwd();
	}
	
	return mvfs_lookup_relative(str_path, pend, current); 	
}

///Стандартные функции для реализации методов fs
extern struct dentry * mvfs_virtual_lookup(struct inode *, const char* name, unsigned int nlen);

__END_DECLS

#endif