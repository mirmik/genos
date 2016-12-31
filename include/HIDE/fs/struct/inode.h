#ifndef GENOS_INODE_H
#define GENOS_INODE_H

#include "inttype.h"

struct super_block;
struct inode_operations;
struct dentry;

struct inode_operations {

struct dentry* (*lookup) ();
int            (*create) ();
//int           (*remove)(struct inode *inode);
int            (*mkdir)  ();
int            (*rmdir)  ();
//	int           (*truncate)(struct inode *inode, size_t len);
//	int           (*pathname)(struct inode *inode, char *buf, int flags);
//	int           (*iterate)(struct inode *next, struct inode *parent, struct dir_ctx *ctx);
//	int           (*rename)(struct inode *node, struct inode *new_parent, const char *new_name);
//	int           (*getxattr)(struct inode *node, const char *name, char *value, size_t size);
//	int           (*setxattr)(struct inode *node, const char *name, const char *value, size_t size, int flags);		
};

struct inode {
//	struct super_block *i_sb;
//	struct dentry *i_dentry;
	uint16_t 					i_no;
	uint16_t 					i_flags;
	uint8_t 					i_nlinks;
	
	struct inode_operations	*	i_ops;
	//struct super_block* 		i_sb;

	//void *						i_data
	//int    start_pos; /* location on disk */
	//size_t length;
	//int    flags;
	//uid_t  i_owner_id;
	//gid_t  i_group_id;
	//mode_t i_mode;

	//private data
};

#endif