#ifndef MVFS_NODE_H
#define MVFS_NODE_H

#include <gxx/datastruct/dlist.h>

#define NAME_LENGTH_MAX 7

struct super_block;
struct file_operations;

struct node {
	struct dlist_head lnk;
	struct dlist_head childrens;
	struct node * parent;
	char name[NAME_LENGTH_MAX + 1];

	struct super_block * i_sb;

	union {
		uint8_t flags;
		struct {
			uint8_t negative_flag : 1;
			uint8_t directory_flag : 1;
			uint8_t special_node_flag : 1;
		};
	};
};

struct special_node {
	struct node node;
	const struct file_operations * f_op;
};

struct node_operations {
	struct node * (*lookup) (struct node * dir, const char * name, size_t nlen);

    int (*mkdir) (struct node * dir, const char * name, size_t nlen, int flags);
    int (*rmdir) (struct node * dir);
};

#endif