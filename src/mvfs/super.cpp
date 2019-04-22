#include <mvfs/super.h>
#include <mvfs/file.h>
#include <mvfs/compat.h>

#include <errno.h>
#include <stdio.h>

#include <assert.h>
#include <igris/dprint.h>

int vfs_open_node(struct node * i, struct file ** filpp) {
	int sts;
	struct file * filp;

	filp = vfs_file_alloc();
	filp->node = i;

	sts = i->open(filp);
		if (sts) {
			vfs_file_dealloc(filp);
			return sts;
		}

	*filpp = filp;

	return 0;
}