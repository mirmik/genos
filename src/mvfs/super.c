#include <mvfs/super.h>
#include <mvfs/file.h>
#include <gxx/panic.h>

#include <errno.h>
#include <stdio.h>

int mvfs_inode_lookup_child(struct inode * inode, 
	const char* name, unsigned int nlen, struct dentry ** retd
) {
	return -ENOTSUP;
	//panic("TODO: mvfs_inode_lookup_child");
}

extern struct file * mvfs_open_inode(struct inode * i) {
	int sts;
	struct file * filp = mvfs_file_alloc();

	const struct file_operations * f_op = mvfs_get_f_ops(i);

	filp->f_op = f_op;
	filp->f_inode = i;

	sts = f_op->open(i, filp);

	return filp;
}