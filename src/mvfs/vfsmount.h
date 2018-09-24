#ifndef MVFS_VFSMOUNT_H
#define MVFS_VFSMOUNT_H

#include <gxx/datastruct/dlist.h>

/**
 *	Описатель точки монтирования.
 *
 */
struct vfsmount {
	struct dlist_head link; /// < линк в хэштаблицу vfsmount.
	struct node * mount_point;
	struct node * root;
	//struct super_block * sb;
	//struct vfsmount * parent;
};

__BEGIN_DECLS

struct vfsmount * vfs_vfsmount_get(struct node * mount_point);
struct vfsmount * vfs_vfsmount_create(struct node * mount_point, struct node * root);

__END_DECLS

#endif