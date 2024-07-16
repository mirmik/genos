/**
 * @file
 * @brief Implementation of methods in util/tree.h
 *
 * @date Oct 8, 2011
 * @author Avdyukhin Dmitry
 */

#include <assert.h>
#include <gxx/datastruct/tree.h>

struct tree_link *tree_link_init(struct tree_link *link) {
	assert(link != NULL);
	dlist_init(&link->children);
	dlist_init(&link->list_link);
	link->par = NULL;
	return link;
}

void tree_add_link(struct tree_link *link, struct tree_link *parent) {
	assert(parent != NULL);
	assert(link != NULL);
	assert(link->par == NULL);
	dlist_add_prev(&link->list_link, &parent->children);
	link->par = parent;
}

void tree_move_link(struct tree_link *link, struct tree_link *parent) {
	assert(link != NULL);
	assert(parent != NULL);
	if (parent != link->par) {
		tree_unlink_link(link);
		tree_add_link(parent, link);
	}
}

int tree_unlink_link(struct tree_link *link) {
	assert(link != NULL);
	if (link->par != NULL) {
		dlist_del_init(&link->list_link);
		link->par = NULL;
		return 1;
	}
	return 0;
}
