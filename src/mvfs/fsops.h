/**
 *	API. Комплексные операции над vfs.
 */

#ifndef MVFS_FOPS_H
#define MVFS_FOPS_H

#include <sys/cdefs.h>

struct path;

__BEGIN_DECLS

extern int vfs_mount_first(const char* fstype, unsigned long mountflags, const void *data);

extern int vfs_mount(const char *source, const char *target, 
	const char* fstype, unsigned long mountflags, const void *data);

extern int vfs_mkdir(const char *path);
extern int vfs_rmdir(const char *path);
extern int vfs_chdir(const char *path);
extern int vfs_chroot(const char *path);

__END_DECLS

#endif