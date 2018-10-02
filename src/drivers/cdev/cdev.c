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

	sts = vfs_lookup(dir, NULL, &parent);
		if (sts) 
			return sts;
	
	
	node_init(&cdev->node.node, name, strlen(name));
	node_add_child(&cdev->node.node, parent);
	cdev->node.f_op = f_op;

	return 0;
}