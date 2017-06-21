#ifndef KERNEL_FS_FILE_H
#define KERNEL_FS_FILE_H

#include <inttypes.h>
#include <compiler.h>

struct file_s;
typedef struct file_s file_t;

typedef int(*file_write_t)(struct file_s*, const void*, size_t);
typedef int(*file_read_t)(struct file_s*, const void*, size_t);

struct file_operations {
	file_write_t write;
	file_read_t read;
};

struct file_s {
	const struct file_operations * fops;
};

__BEGIN_DECLS

static inline void file_init(file_t* filp, const struct file_operations* fops) {
	filp->fops = fops;
}

__END_DECLS

#endif