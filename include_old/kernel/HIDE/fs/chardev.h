#ifndef GENOS_CHARDEV_H
#define GENOS_CHARDEV_H

#include <kernel/fs/file.h>

struct chardev_s;
typedef struct chardev_s chardev_t;

typedef int(*chardev_write_t)(struct chardev_s*, const char*, size_t);
typedef int(*chardev_read_t)(struct chardev_s*, const char*, size_t);
typedef int(*chardev_avail_t)(struct chardev_s*);
typedef int(*chardev_room_t)(struct chardev_s*);

struct chardev_operations {
	chardev_write_t write;
	chardev_read_t read;
	chardev_avail_t avail;
	chardev_room_t room;
};

struct chardev_s {
	file_t fl;
	const struct chardev_operations * cdops;
};

__BEGIN_DECLS

extern const struct file_operations chardev_file_ops;

static inline void __chardev_init(
	chardev_t* cdevp,
	const struct chardev_operations* cdops, 
	const struct file_operations* fops) 
{
	cdevp->cdops = cdops;
	file_init(&cdevp->fl, fops);
}

static inline void chardev_init(chardev_t* cdevp, const struct chardev_operations* cdops) {
	__chardev_init(cdevp, cdops, &chardev_file_ops);
}

__END_DECLS

#endif