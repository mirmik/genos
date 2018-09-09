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

extern void mvfs_init();

extern struct dentry * mvfs_virtual_lookup(struct inode *, const char* name, unsigned int nlen);

/**
 *	Провести процедуру lookup
 */
extern int mvfs_lookup(const char* str_path, struct dentry** result);

extern struct dentry* mvfs_get_root();

__END_DECLS

#endif