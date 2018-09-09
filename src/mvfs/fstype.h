#ifndef MVFS_FSTYPE_H
#define MVFS_FSTYPE_H

#include <gxx/datastruct/dlist.h>

#define FILESYSTEM_NAME_MAX 4

struct super_block;

/**
 *	Тип - заголовок файловой системы.
 */
struct file_system_type {
	char name [FILESYSTEM_NAME_MAX];

	///< Создать суперблок.
	struct super_block *(*get_sb) (struct file_system_type *, int, const void *);	
	void (*kill_sb) (struct super_block *);

	struct dlist_head fs_list;
};

__BEGIN_DECLS

extern struct file_system_type * mvfs_get_fs(const char* name);

extern void mvfs_register_fs(struct file_system_type* fs);

__END_DECLS

#endif