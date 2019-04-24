#include <mvfs/super.h>
#include <mvfs/file.h>
#include <mvfs/compat.h>

#include <errno.h>
#include <stdio.h>

#include <assert.h>
#include <igris/dprint.h>

int vfs_open_node(struct node * i, struct file ** filpp)
{
	int sts;
	struct file * filp;

	filp = vfs_file_alloc();
	filp->node = i;

	sts = i->n_ops->open(i, filp);

	if (sts)
	{
		vfs_file_dealloc(filp);
		return sts;
	}

	*filpp = filp;

	return 0;
}


void super_block_init(struct super_block * sb,
                      const struct file_system_type * _fs,
                      struct node * _root)
{
	sb->fs = _fs;
	sb->root = _root;
	sb->root->flag.directory = 1;
	sb->root->sb = sb;
}