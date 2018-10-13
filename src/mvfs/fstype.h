#ifndef MVFS_FSTYPE_H
#define MVFS_FSTYPE_H

#include <gxx/datastruct/slist.h>

#define FILESYSTEM_NAME_MAX 4

struct super_block;

/**
 *	Тип - заголовок файловой системы
 */
struct file_system_type {
	const char * name;

	///< Создать суперблок.
	virtual struct super_block * get_sb (int, const void *) = 0;	
	virtual void kill_sb(struct super_block *) = 0;

	struct slist_head fs_list;

	file_system_type(const char * _name) : name(_name) {}
};

__BEGIN_DECLS

///Получить ранее проиндексированный file_system_type
extern struct file_system_type * vfs_get_fs(const char* name);

///Проиндексировать объект file_system_type в таблице файловых систем
extern void vfs_register_fs(struct file_system_type* fs);

__END_DECLS

#endif