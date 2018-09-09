#include <drivers/char/char_device.h>
#include <fs/vfs.h>
#include <errno.h>

#include <assert.h>
#include <gxx/debug/dprint.h>

DLIST_HEAD(cdev_list);

int char_device_register(struct char_device *cdev, const char* name, const struct file_operations* ops) {
	debug_print_line(__PRETTY_FUNCTION__);
	struct path path;

	cdev->fops = ops;
	dlist_add(&cdev->cdev_list, &cdev_list);
	
	if (vfs_lookup("/dev", &path)) {
		return -ENOENT;
	}
	assert(path.node);
	
	//vfs_create_child(&path, name, S_IFCHR | S_IRALL | S_IWALL, &node);
	//assert(path.node);

	return 0;
}


/*int char_dev_register(const char *name, const struct kfile_operations *ops) {
	struct path  node;
	struct nas *dev_nas;

	if (vfs_lookup("/dev", &node)) {
		return -ENOENT;
	}

	if (node.node == NULL) {
		return -ENODEV;
	}

	vfs_create_child(&node, name, S_IFCHR | S_IRALL | S_IWALL, &node);
	if (!(node.node)) {
		return -1;
	}

	dev_nas = node.node->nas;
	dev_nas->fs = filesystem_create("empty");
	if (dev_nas->fs == NULL) {
		return -ENOMEM;
	}

	dev_nas->fs->file_op = ops;

	return 0;
}*/
