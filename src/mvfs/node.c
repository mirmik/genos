#include <mvfs/node.h>
#include <mvfs/mvfs.h>
#include <gxx/debug/dprint.h>

static inline void __vfs_dpr_node_tree(struct node * d, const int t) {
	struct node * it;

	int i;

	int tt = t;
	while (tt--) dpr("  "); dprln(d->name);
	dlist_for_each_entry(it, &d->childrens, lnk) {
		__vfs_dpr_node_tree(it, t+1);
	}
}

void vfs_dpr_node_tree(struct node * d) {
	__vfs_dpr_node_tree(d ? d : vfs_get_pwd(), 0);
}
