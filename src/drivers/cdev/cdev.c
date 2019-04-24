#include <drivers/cdev/cdev.h>

#include <mvfs/file.h>
#include <mvfs/mvfs.h>
#include <mvfs/node.h>

#include <mvfs/trace.h>

int vfs_link_cdev(struct char_device * cdev, const char * dir,
	const char* name
) {
	DTRACE();
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

int cdev_common_open(struct node * i, struct file * filpp) 
{
	return 0;
}

int cdev_common_write(struct node * i, struct file * filp, const char* data, unsigned int size, int flags) 
{
	struct char_device * cdev = mcast_out(i, struct char_device, node);
	return cdev->c_ops->write(cdev, data, size, flags);
}

const struct node_operations cdev_node_ops = 
{
	.open = cdev_common_open,
	.write = cdev_common_write
};