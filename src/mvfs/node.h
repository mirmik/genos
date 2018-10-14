#ifndef MVFS_NODE_H
#define MVFS_NODE_H

#include <gxx/container/dlist.h>
#include <stdint.h>
#include <string.h>

#include <errno.h>

#define NAME_LENGTH_MAX 7

struct super_block;
struct file_operations;
struct file;

struct node {
	struct dlist_head lnk;
	gxx::dlist<struct node, &node::lnk> childs;

	struct node * parent;
	char name[NAME_LENGTH_MAX + 1];
	struct super_block * sb;

	union {
		uint8_t flags;
		struct {
			uint8_t negative : 1;
			uint8_t directory : 1;
			uint8_t special_node : 1;
			uint8_t mount_point : 1;
			uint8_t noblock : 1;
			uint8_t nosched : 1;
		} flag;
	};

	//file operations
	virtual int open (struct file*) { return ENOTSUP; } ///< заполнить структуру файла по inode.
	virtual int release (struct file*) { return ENOTSUP; }	
	virtual int write (struct file*, const char* data, unsigned int size) { return ENOTSUP; }
	virtual int read (struct file*, char* data, unsigned int size) { return ENOTSUP; }
	virtual int iterate (struct file*, void*) { return ENOTSUP; }

	//inode operations
    virtual int mkdir(const char * name, size_t nlen, int flags) { return ENOTSUP; }
    virtual int rmdir() { return ENOTSUP; }
};

struct special_node {
	struct node node;
	const struct file_operations * f_op;
};

struct node_operations {
	struct node * (*lookup) (struct node * dir, const char * name, size_t nlen);

};

__BEGIN_DECLS

extern int vfs_open_node(struct node * i, struct file ** filpp);

//struct node * virtual_node_alloc();
//void virtual_node_dealloc(struct node *);

static inline void node_init(struct node * node, const char * name, size_t nlen) {
	int len;

	len = nlen < NAME_LENGTH_MAX ? nlen : NAME_LENGTH_MAX;

	strncpy(node->name, name, len);
	node->name[len] = '\0';
	node->flags = 0;
}

static inline void node_add_child(struct node * node, struct node * parent) {
	parent->childs.add_last(*node);
	//dlist_add(&node->lnk, &parent->childrens);
	node->parent = parent;
}

/*static inline struct node * virtual_node_create(const char * name, size_t nlen) {
	struct node * ret = virtual_node_alloc();
	node_init(ret, name, nlen);
	return ret;
}*/

/*static inline struct node * virtual_node_create_as_child(const char * name, size_t nlen,
	struct node * parent
) {
	struct node * ret = virtual_node_create(name, nlen);
	node_add_child(ret, parent);
	return ret;
}

static inline void virtual_node_release(struct node * node) {
	dlist_del(&node->lnk);
	virtual_node_dealloc(node);
}*/

extern void vfs_dpr_node_tree(struct node * root);

__END_DECLS

#endif