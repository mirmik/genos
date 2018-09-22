#include <mvfs/super.h>
#include <mvfs/file.h>
#include <mvfs/compat.h>
#include <gxx/panic.h>

#include <errno.h>
#include <stdio.h>

#include <assert.h>
#include <gxx/debug/dprint.h>

extern int vfs_open_inode(struct inode * i, struct file ** filpp) {
	int sts;
	struct file * filp;
	const struct file_operations * f_op = vfs_get_f_ops(i);

	if (f_op->open == NULL)
		return ENOTSUP;

	filp = vfs_file_alloc();
	filp->f_op = f_op;
	filp->f_inode = i;

	sts = f_op->open(i, filp);
	if (sts) {
		vfs_file_dealloc(filp);
		return sts;
	}

	*filpp = filp;

	return 0;
}