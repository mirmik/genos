/**
 *	Комплексные операции над mvfs.
 */

#ifndef MVFS_FOPS_H
#define MVFS_FOPS_H

#include <sys/cdefs.h>

struct path;

__BEGIN_DECLS

/// Получить текущий root
extern struct dentry* mvfs_get_root();

extern int mvfs_mount_first(const char* fstype, unsigned long mountflags, const void *data);

extern int mvfs_mount(const char *source, const char *target, 
	const char* fstype, unsigned long mountflags, const void *data);

extern int mvfs_mkdir(const char *path);

__END_DECLS

#endif