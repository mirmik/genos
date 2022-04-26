#ifndef MVFS_SUPER_H
#define MVFS_SUPER_H

/**
 *	Описатели суперблока и индксного дескриптора.
 *
 *	В vfs данные объекты работают в тесной связи. Поэтому айноды включены в этот файл.
 */

//#include <igris/util/member.h>

#include <stdlib.h>
#include <stdint.h>

#include <mvfs/node.h>

struct file;

///	Суперблок смонтированной файловой системы.
struct super_block
{
	struct dlist_head sblocks; 
	
	const struct file_system_type * fs;
	struct node * root; ///корневой dentry ветви.
	uint16_t 		refs;

	struct node * mount_point;
};

__BEGIN_DECLS

void super_block_init(struct super_block * sb,
                      const struct file_system_type * _fs,
                      struct node * _root);

void super_block_add_to_list(struct super_block * sb);

/// Найти точку монтирование соответствующую указанному ноду.
/// В текущей реализации монтирование выполняется непосредственно на суперблоке.
struct super_block * vfs_vfsmount_get(struct node * mount_point);

__END_DECLS

#define SUPER_BLOCK_INIT(name, fs, root) { DLIST_HEAD_INIT(name.sblocks), fs, root, 0, NULL }

#endif