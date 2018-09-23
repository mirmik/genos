#ifndef MVFS_BLOCK_DEVICE_H
#define MVFS_BLOCK_DEVICE_H

#include <mvfs/inode.h>
#include <stdlib.h>

struct block_device_operations;

struct block_device {
	struct special_inode i;

	const struct block_device_operations * blk_op;
};

struct block_device_operations {
	int (*write_block) (struct block_device *, char * buf, size_t sz, int blkno);
	int (*read_block) (struct block_device *, char * buf, size_t sz, int blkno);
};

#endif