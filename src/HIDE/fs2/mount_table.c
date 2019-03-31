#include <fs/mount.h>

#include <gxx/debug/dprint.h>

/*#include <assert.h>
#include <string.h>
#include <errno.h>

#include <igris/datastruct/dlist.h>
#include <mem/misc/pool.h>

#include <fs/node.h>
#include <fs/vfs.h>
#include <fs/mount.h>

#include <mem/misc/pool.h>

#ifndef MOUNT_DESC_QUANTITY
#	define MOUNT_DESC_QUANTITY 2
#endif

POOL_DEF(mount_desc_pool, struct mount_descriptor, MOUNT_DESC_QUANTITY);
*/
static struct mount *mnt_root = NULL;
/*
struct mount_descriptor *mount_table_get_child(struct mount_descriptor *parent, struct node *mnt_point) {
	struct mount_descriptor *desc;

	if (parent->mnt_point == mnt_point ) {
		assert(parent->mnt_parent == parent);
		return parent;
	}

	dlist_for_each_entry(desc, &parent->mnt_mounts, mnt_child) {
		if (desc->mnt_point == mnt_point) {
			return desc;
		}
	}

	return NULL;
}

struct mount_descriptor *mount_table_add(struct path *mnt_point_path,
		struct node *root, const char *dev) {
	struct mount_descriptor *mdesc;

	assert(mnt_point_path->mnt_desc != NULL ||
			(mnt_point_path->mnt_desc == NULL && mnt_root == NULL));

	if(mnt_point_path->node == NULL) {
		return NULL;
	}

	if(mnt_root != NULL && mnt_point_path->node ==
			mnt_point_path->mnt_desc->mnt_root) {
		return NULL;
	}

	if(NULL == (mdesc = pool_alloc(&mount_desc_pool))) {
		return NULL;
	}

	mdesc->mnt_point = mnt_point_path->node;
	mdesc->mnt_point->mounted++;
	mdesc->mnt_root = root;

	/* XXX mount root should preserve mode, uid/gid */
	/*root->mode = mnt_point_path->node->mode;
	root->uid = mnt_point_path->node->uid;
	root->gid = mnt_point_path->node->gid;

	dlist_init(&mdesc->mnt_mounts);
	dlist_head_init(&mdesc->mnt_child);

	if (mnt_point_path->mnt_desc == NULL) {
		mdesc->mnt_parent = mdesc;
		mnt_root = mdesc;
	} else {
		mdesc->mnt_parent = mnt_point_path->mnt_desc;
		dlist_add_next(&mdesc->mnt_child, &mnt_point_path->mnt_desc->mnt_mounts);
	}

	strncpy(mdesc->mnt_dev, dev, MOUNT_DESC_STRINFO_LEN);
	mdesc->mnt_dev[MOUNT_DESC_STRINFO_LEN - 1] = '\0';

	return mdesc;
}

int mount_table_del(struct mount_descriptor *mdesc) {

	if(mdesc == NULL) {
		return -EINVAL;
	}

	if (mnt_root == mdesc) {
		mnt_root = NULL;
	}

	mdesc->mnt_point->mounted--;
	vfs_del_leaf(mdesc->mnt_root);

	dlist_del(&mdesc->mnt_mounts);
	dlist_del(&mdesc->mnt_child);

	pool_free(&mount_desc_pool, mdesc);

	return 0;
}
*/
struct mount *mount_table(void) {
	debug_print_line(__PRETTY_FUNCTION__);
	return mnt_root;
}
