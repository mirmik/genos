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

/// Индексный дескриптор файла.
struct inode {
	struct super_block *      i_sb; ///Указатель на суперблок, к которому относится данный inode.
	uint16_t                  nlink;
	union {
		uint8_t flags; 
		struct {
			uint8_t directory_flag : 1;
		};
	};
};

///Один из специальных типов файлов.
struct special_inode {
	struct inode i;
	const struct file_operations* f_op;
};

struct inode_operations {
//super_block operations:
	//struct inode *(*alloc_inode)(struct super_block *sb); ///< Выделить inode с приватными данными, специфическими для fs.
	//int           (*destroy_inode)(struct inode *inode); ///< Парная предыдущей операция деаллокации.

//inode operations: 
	///Создать inode для переданной dentry
	//int (*create) (struct inode *, struct dentry *, uint8_t);

	///Процедура lookup. В отличие от большого vfs, с целью экономии памяти может ничего не возвращать.
	///Принимает родительский inode. Может создать пару inode-dentry, или вернуть негативный dentry.
	struct dentry * (*lookup) (struct inode *, struct dentry *);

	///Оторвать dentry от inode.
	//int (*unlink) (struct inode *,struct dentry *);

	//revalidate - вопрос на закэшированную запись.


    int (*mkdir) (struct inode *, struct dentry *,int);
    int (*rmdir) (struct inode *, struct dentry *);
};


__BEGIN_DECLS

static inline const struct file_operations * vfs_get_f_ops(struct inode* i) {
	if (i->i_sb == NULL) 
		return mcast_out(i, struct special_inode, i)->f_op;
	else return i->i_sb->f_op;
}

extern int vfs_open_inode(struct inode * i, struct file ** filpp);

#endif