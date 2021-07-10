/*#include <drivers/cdev/serdev.h>

#include <mvfs/file.h>
#include <mvfs/mvfs.h>
#include <mvfs/node.h>

#include <mvfs/trace.h>

int vfs_link_cdev(struct serial_device * cdev, const char * dir,
	const char* name
) {
	DTRACE();
	int sts;
	struct node * parent;

	sts = vfs_lookup(dir, NULL, &parent);
		if (sts) 
			return sts;
	
	
	//node_init(&cdev->node, name, strlen(name));
	node_add_child(&serdev->node, parent);

	return 0;
}

void serhar_device_init(struct serial_device * serdev, 
	const struct serial_device_operations* ops) 
{
	serdev -> c_ops = ops;
}

int serdev_common_open(struct node * i, struct file * filpp) 
{
	return 0;
}

int serdev_common_write(struct node * i, struct file * filp, const char* data, unsigned int size, int flags) 
{
	struct serial_device * serdev = mcast_out(i, struct serial_device, node);
	return serdev->c_ops->write(serdev, data, size, flags);
}

const struct node_operations cdev_node_ops = 
{
	.open = serdev_common_open,
	.write = serdev_common_write
};*/