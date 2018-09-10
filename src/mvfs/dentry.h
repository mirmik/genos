#ifndef MVFS_DENTRY_H
#define MVFS_DENTRY_H

/**
 *	В этом файле операции над деревом dentry и сопутствующими объектами.
 *
 *	В реализации операций убраны всяческие упоминания других объектов mvfs, включая inode.
 *	Здесь только операции над деревом.
 */

#include <gxx/datastruct/tree.h>
#include <stdint.h>
#include <string.h>

#define NAME_LENGTH_MAX 7

struct inode;

struct dentry {
	struct tree_link 	d_tree;
	struct inode * 		d_inode;
	char 				name [NAME_LENGTH_MAX + 1];

	union {
		uint8_t flags;
		struct {
			uint8_t mount_point_flag : 1;
		};
	};
	uint8_t             refs;
};

__BEGIN_DECLS

///Выделить новый дентри без инициализации. (Из пула, или из кучи, зависит от реализации.)
extern struct dentry * mvfs_dentry_alloc();

///Парный вызов. Деалоцировать дентри.
extern void mvfs_dentry_dealloc(struct dentry *);

///Аллоцировать dentry и провести инициализацию с установкой имени.
extern struct dentry * mvfs_dentry_create_n(const char* name, unsigned int nlen);
static inline struct dentry * mvfs_dentry_create(const char* name) {
	return mvfs_dentry_create_n(name, strlen(name));
}

///Добавить 
extern void mvfs_dentry_add_child(struct dentry* child, struct dentry* parent);

/**
 *	Найти dentry с именем в списке потомков parent. 
 *	
 *	Возвращает найденный dentry, или NULL, если в списке такого нет.  
 *  Может реализовываться через хэштаблицу.
 */
extern struct dentry * mvfs_dentry_lookup_child(struct dentry* parent, const char* name, unsigned int nlen);

__END_DECLS

#endif