#ifndef MVFS_CDEV_H
#define MVFS_CDEV_H

#include <mvfs/node.h>

struct char_device : public node {
	
};

__BEGIN_DECLS

//extern void char_device_init(struct char_device * cdev);

extern int vfs_link_cdev(struct char_device * cdev, const char * dir,
	const char* name);

__END_DECLS

#endif