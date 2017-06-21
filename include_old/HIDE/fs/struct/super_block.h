#ifndef GENOS_SUPERBLOCK_H
#define GENOS_SUPERBLOCK_H

#include <inttypes.h>
#include <defines/atomic_t.h>
#include <genos/datastruct/dlist_head.h>

struct file_system_type;
struct inode;

struct super_operations {
	//struct inode *	(*alloc_inode)	(struct super_block *sb);
	//int 			(*destroy_inode)(struct inode *inode);
	int 			(*read_inode)	(struct inode *inode);
	//int 			(*write_inode)	(struct inode *inode, int flag);
	//int 			(*delete_inode)	(struct inode *inode);
	//int 			(*umount_begin)	(struct super_block *sb);
	//struct idesc *	(*open_idesc)	(struct lookup *l);
};

struct super_block {
	size_t inode_size;

	//const struct file_system_type *fs_type;
	//struct block_dev 				*bdev;
	//struct file 					*bdev_file;
	//struct dlist_head 			*inode_list;
	
	uint8_t flags;
	atomic_t refs;
	//void *private;
	struct super_operations *sb_ops;
	struct dentry 			*root;
	
	//struct inode_operations	      *sb_iops;
	//struct dentry_operations      *sb_dops;
	//struct file_operations        *sb_fops;

	//private data
};

#endif