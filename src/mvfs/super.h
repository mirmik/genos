#ifndef MVFS_SUPER_H
#define MVFS_SUPER_H

/**
 *	Описатели суперблока и индксного дескриптора.
 *
 *	В vfs данные объекты работают в тесной связи. Поэтому айноды включены в этот файл. 
 */

#include <gxx/util/member.h>

#include <stdlib.h>
#include <stdint.h>

#include <mvfs/inode.h>

struct dentry;
struct file;

///	Суперблок смонтированной файловой системы.
struct super_block {
	const struct file_system_type * s_fs;
	struct dentry *  s_root; ///корневой dentry ветви.

	//void *			private;
	uint16_t 		refs;

	const struct inode_operations * i_op; ///Виртуальная таблица операций над объектами fs.
	const struct file_operations  * f_op; ///Операции ввода-вывода и управления.
};

__BEGIN_DECLS

static inline const struct file_operations * vfs_get_f_ops(struct inode* i) {
	if (i->i_sb == NULL) 
		return mcast_out(i, struct special_inode, i)->f_op;
	else return i->i_sb->f_op;
}

__END_DECLS

#endif