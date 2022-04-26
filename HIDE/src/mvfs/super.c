#include <mvfs/super.h>
#include <mvfs/file.h>
#include <mvfs/compat.h>

#include <errno.h>
#include <stdio.h>

#include <assert.h>
#include <igris/dprint.h>

DLIST_HEAD(super_block_list);

void super_block_init(struct super_block * sb,
                      const struct file_system_type * _fs,
                      struct node * _root)
{
	sb->fs = _fs;
	sb->root = _root;
	sb->root->flag.directory = 1;
	sb->root->sb = sb;
	
	dlist_add(&sb->sblocks, &super_block_list);
}

void super_block_add_to_list(struct super_block * sb) 
{
	dlist_add(&sb->sblocks, &super_block_list);
}

/**
 *	В качестве ключа для поиска в таблице точек монтирования используется нод точки монтирования.
 */
struct super_block * vfs_vfsmount_get(struct node * mount_point) 
{
	struct super_block * it;

	dlist_for_each_entry(it, &super_block_list, sblocks) 
	{
		if (it->mount_point == mount_point) 
			return it;
	}

	return NULL;
}