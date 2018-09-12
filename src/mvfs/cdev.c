#include <mvfs/cdev.h>
#include <mvfs/file.h>
#include <mvfs/mvfs.h>
#include <mvfs/dentry.h>
#include <gxx/panic.h>

int mvfs_link_cdev(struct char_device * cdev, 
	const struct file_operations * f_op, const char * dir,
	const char* name
) {
	int sts;
	struct dentry * parent;

	if (sts = mvfs_lookup(dir, &dir, &parent)) { 
		return sts;
	}

	cdev->c_i.f_op = f_op;
	struct dentry * node = mvfs_dentry_create(name);
	node->d_inode = &cdev->c_i.i;
	mvfs_dentry_add_child(node, parent);

	return 0;
}