#include <drivers/cdev/cdev.h>

#include <mvfs/file.h>
#include <mvfs/mvfs.h>
#include <mvfs/node.h>

#include <gxx/panic.h>

int vfs_link_cdev(struct char_device * cdev, 
	const struct file_operations * f_op, const char * dir,
	const char* name
) {
	int sts;
	struct node * parent;

	if (sts = vfs_lookup(dir, NULL, &parent)) { 
		return sts;
	}

	cdev->node.f_op = f_op;
	parent = &cdev->node.node;
	
	return 0;
}