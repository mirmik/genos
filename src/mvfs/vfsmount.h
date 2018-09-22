#ifndef MVFS_VFSMOUNT_H
#define MVFS_VFSMOUNT_H

#include <gxx/datastruct/dlist.h>

/**
 *	Описатель точки монтирования.
 *
 */
struct vfsmount {
	struct dlist_head link; /// < линк в хэштаблицу vfsmount.
	struct dentry * mount_point;
	struct dentry * root;
	//struct super_block * sb;
	//struct vfsmount * parent;
};

__BEGIN_DECLS

struct vfsmount * vfs_vfsmount_get(struct dentry * mount_point);
struct vfsmount * vfs_vfsmount_create(struct dentry * mount_point, struct dentry * root);

__END_DECLS

#endif