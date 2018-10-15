#ifndef MVFS_CDEV_H
#define MVFS_CDEV_H

#include <mvfs/node.h>

struct char_device : public node 
{
	virtual int write(const char * data, unsigned int size);
	virtual int read(char * data, unsigned int size);

	int write(struct file * filp, const char * data, unsigned int size) override {
		return write(data, size);
	}	

	int read(struct file * filp, char * data, unsigned int size) override {
		return read(data, size);
	}

	virtual int waitread() {}
};

__BEGIN_DECLS

//extern void char_device_init(struct char_device * cdev);

extern int vfs_link_cdev(struct char_device * cdev, const char * dir,
	const char* name);

__END_DECLS

#endif