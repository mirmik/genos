#ifndef MVFS_NODE_H
#define MVFS_NODE_H

#include <gxx/datastruct/dlist.h>
#include <stdint.h>
#include <string.h>

#define NAME_LENGTH_MAX 7

struct super_block;
struct file_operations;
struct file;

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
			uint8_t mount_point_flag : 1;
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

__BEGIN_DECLS

extern int vfs_open_node(struct node * i, struct file ** filpp);

struct node * virtual_node_alloc();
void virtual_node_dealloc(struct node *);

static inline struct node * virtual_node_create(const char * name, size_t nlen) {
	struct node * ret = virtual_node_alloc();
	dlist_init(&ret->childrens);
	ret->flags = 0;
	strncpy(ret->name, name, nlen < NAME_LENGTH_MAX ? nlen : NAME_LENGTH_MAX);
	return ret;
}

static inline struct node * virtual_node_create_as_child(const char * name, size_t nlen,
	struct node * parent
) {
	struct node * ret = virtual_node_create(name, nlen);
	dlist_add(&ret->lnk, &parent->childrens);
	ret->parent = parent;
}

static inline void virtual_node_release(struct node * node) {
	dlist_del(&node->lnk);
	virtual_node_dealloc(node);
}

extern void vfs_dpr_node_tree(struct node * root);

__END_DECLS

#endif