#ifndef MVFS_CDEV_H
#define MVFS_CDEV_H

#include <mvfs/ioflags.h>

#ifdef MVFS_INCLUDED
#include <mvfs/node.h>
#else
#include <mvfs/nodestub.h>
#endif

#ifdef __cplusplus
/*#include <mvfs/node.h>

namespace genos
{
	struct char_device : public node
	{
		virtual int write(const char * data, unsigned int size) = 0;
		virtual int read(char * data, unsigned int size) = 0;

		int write(struct file * filp, const char * data, unsigned int size) override
		{
			return write(data, size);
		}

		int read(struct file * filp, char * data, unsigned int size) override
		{
			return read(data, size);
		}

		virtual int waitread() {}
	};
}*/

#endif

/*#ifdef MVFS_INCLUDED
# include <mvfs/node.h>
#else
# include <mvfs/nodestub.h>
#endif*/

struct char_device;

typedef int(*char_write_t)(struct char_device *, const char* data,
                           unsigned int len, int flags);

typedef int(*char_read_t)(struct char_device *, char* data,
                          unsigned int len, int flags);

typedef int(*char_waitread_t)(struct char_device *);

typedef int(*char_room_t)(struct char_device *);

typedef int(*char_avail_t)(struct char_device *);

struct char_device_operations
{
	char_write_t 		write;		///< Записать данные в устройство
	char_read_t 		read;		///< Считать наличествующие данные
	char_room_t			room;
	char_avail_t		avail;
	char_waitread_t 	waitread;	///< Ожидать появления данных
};

struct char_device
{
#ifdef MVFS_INCLUDED
	struct node node;
#else
	struct nodestub node;
#endif

	const struct char_device_operations * c_ops;
};

#ifdef MVFS_INCLUDED
# define CHAR_DEVICE_INIT(name, ops) {NODE_INIT(name.node), ops}
#else
# define CHAR_DEVICE_INIT(name, ops) {NODESTUB_INIT(name.node), ops}
#endif

__BEGIN_DECLS

//extern void char_device_init(struct char_device * cdev);

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

static inline
int cdev_waitread(struct char_device * dev)
{
	return dev->c_ops->waitread(dev);
}

__END_DECLS

#endif