#ifndef GENOS_MVFS_MVFS_H
#define GENOS_MVFS_MVFS_H

/**
 *	Попытка написать минимальный vfs.
 *
 */

#include <stdlib.h>
#include <stdint.h>

struct inode;
struct dentry;


__BEGIN_DECLS

/// Получить текущий root
extern struct dentry* mvfs_get_root();
extern struct dentry* mvfs_get_pwd();

extern void mvfs_init();

///Провести процедуру lookup от корня.
extern int mvfs_lookup(const char* str_path, const char** pend, struct dentry** current);

///Стандартные функции для реализации методов fs
extern struct dentry * mvfs_virtual_lookup(struct inode *, const char* name, unsigned int nlen);
__END_DECLS

#endif