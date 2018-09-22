#include <mvfs/dentry.h>
#include <mvfs/mvfs.h>

#include <string.h>

#include <stdlib.h>
#include <stdio.h>

struct dentry * vfs_dentry_alloc() {
	return (struct dentry *) malloc(sizeof(struct dentry));
}

void vfs_dentry_dealloc(struct dentry * d) {
	free(d);
}

struct dentry * vfs_dentry_create_n(const char* name, unsigned int nlen) {
	struct dentry * ret = vfs_dentry_alloc();

	strncpy(ret->name, name, nlen < NAME_LENGTH_MAX ? nlen : NAME_LENGTH_MAX);
	dlist_init(&ret->childrens);

	ret->d_inode = NULL;	

	return ret;
}

void vfs_dentry_add_child(struct dentry* child, struct dentry* parent) {
	vfs_lock();
	dlist_add(&child->lnk, &parent->childrens);
	child->parent = parent;
	vfs_unlock();
}

static inline void __vfs_dpr_dentry_tree(struct dentry * d, const int t) {
	struct dentry * it;

	int i;

	int tt = t;
	while (tt--) dpr("  "); dprln(d->name);
	dlist_for_each_entry(it, &d->childrens, lnk) {
		__vfs_dpr_dentry_tree(it, t+1);
	}
}

void vfs_dpr_dentry_tree(struct dentry * d) {
	__vfs_dpr_dentry_tree(d, 0);
}

void vfs_dentry_clean(struct dentry * d) 
{
	dlist_del(&d->lnk);
	vfs_dentry_dealloc(d);
}