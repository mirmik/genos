#ifndef MVFS_BLOCK_DEVICE_H
#define MVFS_BLOCK_DEVICE_H

#include <mvfs/node.h>
#include <stdlib.h>

struct block_device_operations;

struct block_device {
	struct special_node node;

	const struct block_device_operations * blk_op;
};

struct block_device_operations {
	int (*write_block) (struct block_device *, char * buf, size_t sz, size_t blkno);
	int (*read_block) (struct block_device *, char * buf, size_t sz, size_t blkno);
	int (*ioctl) (struct block_device *bdev, int cmd, void *buf, size_t size);
};

#endif