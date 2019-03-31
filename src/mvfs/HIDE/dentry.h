#ifndef MVFS_DENTRY_H
#define MVFS_DENTRY_H

/**
 *	В этом файле операции над деревом dentry и сопутствующими объектами.
 *
 *	В реализации операций убраны всяческие упоминания других объектов vfs, включая inode.
 *	Здесь только операции над деревом.
 */

#include <igris/datastruct/dlist.h>
#include <gxx/debug/dprint.h>
#include <stdint.h>
#include <string.h>

#define NAME_LENGTH_MAX 7

struct inode;

struct dentry {
	//struct tree_link 	d_tree;
	struct dlist_head 	lnk;
	struct dlist_head 	childrens;
	struct dentry * parent;

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

///Аллоцировать dentry и провести инициализацию с установкой имени.
extern struct dentry * vfs_dentry_create_n(const char* name, unsigned int nlen);
static inline struct dentry * vfs_dentry_create(const char* name) {
	return vfs_dentry_create_n(name, strlen(name));
}

///Добавить 
extern void vfs_dentry_add_child(struct dentry* child, struct dentry* parent);

/**
 *	Найти dentry с именем в списке потомков parent. 
 *	
 *	Возвращает найденный dentry, или NULL, если в списке такого нет.  
 *  Может реализовываться через хэштаблицу.
 */
extern struct dentry * vfs_dentry_lookup_child(struct dentry* parent, const char* name, unsigned int nlen);

static inline void dpr_dentry(struct dentry * d) {
	dprf("dentry(name: %s, parent: %s)\n", d->name, d->parent ? d->parent->name : NULL);
}

extern void vfs_dpr_dentry_tree(struct dentry * d);

extern void vfs_dentry_clean(struct dentry * d);

__END_DECLS

#endif