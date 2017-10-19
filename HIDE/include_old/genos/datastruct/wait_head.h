#ifndef GENOS_WAITQUEUE_HEAD
#define GENOS_WAITQUEUE_HEAD

#include "inttypes.h"
#include "genos/datastruct/dlist_head.h"
#include "util/bits.h"
#include <sys/cdefs.h>

#define WAITQUEUE_EXCLUSIVE 0x01
#define WAITQUEUE_AUTOREMOVE 0x02

typedef void(* wait_func_t )(uintptr_t);

struct waitqueue_head
{
	dlist_head queue;
};

struct waitqueue_node
{
	dlist_head lst;
	uint8_t flag;
	wait_func_t func;
	uintptr_t param;
};

__BEGIN_DECLS

void waitqueue_head_init(struct waitqueue_head* head)
{
	dlist_init(&head->queue);
}

void waitqueue_node_init(struct waitqueue_node* node, uint8_t flag)
{
	node->flag = flag;
};

void waitqueue_add_exclusive(struct waitqueue_head* head, struct waitqueue_node* node)
{
	assert(node);
	assert(head);
	assert(node->func);
	
	bits_set(node->flag,WAITQUEUE_EXCLUSIVE);
	dlist_add_prev(&head->queue, &node->lst);
}

void waitqueue_add(struct waitqueue_head* head, struct waitqueue_node* node)
{
	assert(node);
	assert(head);
	assert(node->func);
	
	bits_clr(node->flag,WAITQUEUE_EXCLUSIVE);
	dlist_add_next(&head->queue, &node->lst);
}

void waitqueue_execute(waitqueue_head* head)
{
	uint8_t flag;
	assert(head);

	struct waitqueue_node* pos;
	dlist_for_each_entry(pos, &head->queue, lst)
	{
		flag = pos->flag;
		if (bits_mask(flag, WAITQUEUE_AUTOREMOVE)) dlist_del(&pos->lst);
		pos->func(pos->param);
		if (bits_mask(flag, WAITQUEUE_EXCLUSIVE)) break;
	};
};

__END_DECLS

#endif