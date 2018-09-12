#include <mvfs/dentry.h>
#include <mvfs/mvfs.h>

#include <string.h>

#include <stdlib.h>
#include <stdio.h>

struct dentry * mvfs_dentry_alloc() {
	return (struct dentry *) malloc(sizeof(struct dentry));
}

void mvfs_dentry_dealloc(struct dentry * d) {
	free(d);
}

struct dentry * mvfs_dentry_create_n(const char* name, unsigned int nlen) {
	struct dentry * ret = mvfs_dentry_alloc();

	//if (ret != NULL) 
	
	strncpy(ret->name, name, nlen < NAME_LENGTH_MAX ? nlen : NAME_LENGTH_MAX);
	tree_link_init(&ret->d_tree);
	ret->d_inode = NULL;	

	return ret;
}

void mvfs_dentry_add_child(struct dentry* child, struct dentry* parent) {
	mvfs_lock();
	tree_add_link(&child->d_tree, &parent->d_tree);
	mvfs_unlock();
}

struct dentry * mvfs_dentry_lookup_child(struct dentry* parent, const char* name, unsigned int nlen) {
	struct dentry* node;
	
	mvfs_lock();
	tree_for_each_child_entry(node, &parent->d_tree, d_tree) {
		int node_nlen = strlen(node->name);
		if (node_nlen == nlen && strncmp(name, node->name, nlen) == 0) return node;
	}
	mvfs_unlock();

	return NULL;
}