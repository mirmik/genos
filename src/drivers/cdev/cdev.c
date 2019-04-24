#include <drivers/cdev/cdev.h>

#include <mvfs/file.h>
#include <mvfs/mvfs.h>
#include <mvfs/node.h>

int vfs_link_cdev(struct char_device * cdev, const char * dir,
	const char* name
) {
	int sts;
	struct node * parent;

	sts = vfs_lookup(dir, NULL, &parent);
		if (sts) 
			return sts;
	
	
	node_init(&cdev->node, name, strlen(name));
	node_add_child(&cdev->node, parent);

	return 0;
}

void char_device_init(struct char_device * cdev, 
	const struct char_device_operations* ops) 
{
	cdev -> c_ops = ops;
}

const struct node_operations cdev_node_ops = 
{

};