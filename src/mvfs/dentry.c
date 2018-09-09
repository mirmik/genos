#include <mvfs/dentry.h>
#include <string.h>

#include <stdlib.h>

struct dentry * mvfs_dentry_alloc() {
	return (struct dentry *) malloc(sizeof(struct dentry));
}

void mvfs_dentry_dealloc(struct dentry * d) {
	free(d);
}

struct dentry * mvfs_dentry_create(const char* name, unsigned int nlen) {
	struct dentry * ret = mvfs_dentry_alloc();
	if (ret != NULL) 
		strncpy(ret->name, name, nlen < NAME_LENGTH_MAX ? nlen : NAME_LENGTH_MAX);
	return ret;
}

void mvfs_dentry_add_child(struct dentry* child, struct dentry* parent) {
	tree_add_link(&child->d_tree, &parent->d_tree);
}

struct dentry * mvfs_dentry_find_child(struct dentry* parent, const char* name, unsigned int nlen) {
	struct dentry* node;
	tree_for_each_child_entry(node, &parent->d_tree, d_tree) {
		if (strncmp(name, node->name, nlen) == 0) return node;
	}
	return NULL;
}