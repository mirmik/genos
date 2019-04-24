#ifndef MVFS_CDEV_H
#define MVFS_CDEV_H

#include <defs/ioflags.h>

#ifdef MVFS_INCLUDED
#include <mvfs/node.h>
#endif

struct char_device;

typedef int(*char_write_t)(struct char_device *, const char* data,
                           unsigned int len, int flags);

typedef int(*char_read_t)(struct char_device *, char* data,
                          unsigned int len, int flags);

//typedef int(*char_waitread_t)(struct char_device *);

typedef int(*char_room_t)(struct char_device *);

typedef int(*char_avail_t)(struct char_device *);

struct char_device_operations
{
	char_write_t 		write;		///< Записать данные в устройство
	char_read_t 		read;		///< Считать наличествующие данные
	char_room_t			room;
	char_avail_t		avail;
//	char_waitread_t 	waitread;	///< Ожидать появления данных
};

extern const struct node_operations cdev_node_ops;

struct char_device
{
#ifdef MVFS_INCLUDED
	struct node node;
#endif

	const struct char_device_operations * c_ops;
};

#ifdef MVFS_INCLUDED
# define CHAR_DEVICE_INIT(name, cname, sb, cops) \
	{ NODE_INIT(name.node, cname, sb, &cdev_node_ops, NODE_SPECIAL), cops }
#else
# define CHAR_DEVICE_INIT(name, cops) {cops}
#endif

#define CHAR_DEVICE(name, cname, sb, ops) \
struct char_device name = CHAR_DEVICE_INIT(name, cname, sb, ops)

__BEGIN_DECLS

extern void char_device_init(struct char_device * cdev, 
	const struct char_device_operations* ops);

extern int vfs_link_cdev(struct char_device * cdev, const char * dir,
                         const char* name);

static inline
int cdev_write(struct char_device * dev, const char* data,
               unsigned int len, int flags)
{
	return dev->c_ops->write(dev, data, len, flags);
}

static inline
int cdev_read(struct char_device * dev, char* data,
              unsigned int len, int flags)
{
	return dev->c_ops->read(dev, data, len, flags);
}

/*static inline
int cdev_waitread(struct char_device * dev)
{
	return dev->c_ops->waitread(dev);
}*/

__END_DECLS

#endif