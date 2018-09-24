#include <mvfs/mvfs.h>
#include <mvfs/vfsmount.h>
#include <gxx/datastruct/stable.h>

#include <stdlib.h>

DLIST_HEAD(vfsmount_list);

struct vfsmount * vfs_vfsmount_alloc() 
{
	return (struct vfsmount *) malloc(sizeof(struct vfsmount));
}

void vfs_vfsmount_destroy(struct vfsmount * m) 
{
	free(m);
}

/**
 *	В качестве ключа для поиска в таблице точек монтирования используется дентри точки монтирования.
 */
struct vfsmount * vfs_vfsmount_get(struct node * mount_point) 
{
	struct vfsmount * it;

	dlist_for_each_entry(it, &vfsmount_list, link) {
		if (it->mount_point == mount_point) return it;
	}

	return NULL;
}

struct vfsmount * vfs_vfsmount_create(struct node * mount_point, struct node * root) 
{
	struct vfsmount * ret;

	ret = vfs_vfsmount_alloc();
	ret->mount_point = mount_point;
	ret->root = root;
	
	return ret;
}