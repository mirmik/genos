/**
 *	Комплексные операции над mvfs.
 */

#ifndef MVFS_FOPS_H
#define MVFS_FOPS_H

#include <sys/cdefs.h>

struct path;

__BEGIN_DECLS

extern int mvfs_mount_first(const char* fstype, unsigned long mountflags, const void *data);

extern int mvfs_mount(const char *source, const char *target, 
	const char* fstype, unsigned long mountflags, const void *data);

extern int mvfs_mkdir(const char *path);

extern int mvfs_chdir(const char *path);

extern int mvfs_chroot(const char *path);

__END_DECLS

#endif