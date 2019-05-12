#ifndef MVFS_CDEV_H
#define MVFS_CDEV_H

#include <defs/ioflags.h>

#ifdef MVFS_INCLUDED
#include <mvfs/node.h>
#endif

struct serial_device;

typedef int(*char_write_t)(struct serial_device *, const char* data,
                           unsigned int len, int flags);

typedef int(*char_read_t)(struct serial_device *, char* data,
                          unsigned int len, int flags);

//typedef int(*char_waitread_t)(struct serial_device *);

typedef int(*char_room_t)(struct serial_device *);

typedef int(*char_avail_t)(struct serial_device *);

struct serial_device_operations
{
	char_write_t 		write;		///< Записать данные в устройство
	char_read_t 		read;		///< Считать наличествующие данные
	char_room_t			room;
	char_avail_t		avail;
};

extern const struct node_operations cdev_node_ops;

struct serial_device
{
#ifdef MVFS_INCLUDED
	struct node node;
#endif

	const struct serial_device_operations * c_ops;
};

#ifdef MVFS_INCLUDED
# define SERIAL_DEVICE_INIT(name, cname, sb, cops) \
	{ NODE_INIT(name.node, cname, sb, &cdev_node_ops, NODE_SPECIAL), cops }
#else
# define SERIAL_DEVICE_INIT(name, cops) {cops}
#endif

#define SERIAL_DEVICE(name, cname, sb, ops) \
struct serial_device name = SERIAL_DEVICE_INIT(name, cname, sb, ops)

__BEGIN_DECLS

extern void serial_device_init(struct serial_device * cdev, 
	const struct serial_device_operations* ops);

extern int vfs_link_cdev(struct serial_device * cdev, const char * dir,
                         const char* name);

static inline
int serdev_write(struct serial_device * dev, const char* data,
               unsigned int len, int flags)
{
	return dev->c_ops->write(dev, data, len, flags);
}

static inline
int serdev_read(struct serial_device * dev, char* data,
              unsigned int len, int flags)
{
	return dev->c_ops->read(dev, data, len, flags);
}

/*static inline
int cdev_waitread(struct serial_device * dev)
{
	return dev->c_ops->waitread(dev);
}*/

__END_DECLS

#endif