#ifndef GENOS_DATASTRUCT_H
#define GENOS_DATASTRUCT_H

#include <sys/cdefs.h>
#include <datastruct/dlist_head.h>

struct tree_node {
	dlist_head 	lnk;
	tree_node*	parent;
	dlist_head	childs;
};

__BEGIN_DECLS

static inline void tree_init(struct tree_node* node) {
	dlist_init(&node->childs);
	node->parent = 0;
}

static inline void tree_add_child_next(struct tree_node* node, struct tree_node* parent) {
	dlist_add_next(&node->lnk, &parent->childs);
	node->parent = parent;
}

__END_DECLS

#endif