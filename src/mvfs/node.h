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

static inline void node_init(struct node * node, const char * name, size_t nlen) {
	dlist_init(&node->childrens);
	strncpy(node->name, name, nlen < NAME_LENGTH_MAX ? nlen : NAME_LENGTH_MAX);
	node->flags = 0;
}

static inline void node_add_child(struct node * node, struct node * parent) {
	dlist_add(&node->lnk, &parent->childrens);
	node->parent = parent;
}

static inline struct node * virtual_node_create(const char * name, size_t nlen) {
	struct node * ret = virtual_node_alloc();
	node_init(ret, name, nlen);
	return ret;
}

static inline struct node * virtual_node_create_as_child(const char * name, size_t nlen,
	struct node * parent
) {
	struct node * ret = virtual_node_create(name, nlen);
	node_add_child(ret, parent);
}

static inline void virtual_node_release(struct node * node) {
	dlist_del(&node->lnk);
	virtual_node_dealloc(node);
}

extern void vfs_dpr_node_tree(struct node * root);

__END_DECLS

#endif