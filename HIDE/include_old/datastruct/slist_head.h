#ifndef DATASTRUCT_SINGLE_LIST_HEAD_H
#define DATASTRUCT_SINGLE_LIST_HEAD_H

#include <sys/cdefs.h>

struct slist_head {
	slist_head * next;
};

__BEGIN_DECLS

static inline void slist_init(struct slist_head * node) {
	node->next = 0;
}

static inline void slist_add_next(struct slist_head * node, struct slist_head * list) {
	node->next = list->next;
	list->next = node;
}

static inline void slist_del(struct slist_head * del, struct slist_head * prev) {
	prev->next = del->next;
}


__END_DECLS

#endif