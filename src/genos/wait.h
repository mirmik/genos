#ifndef GENOS_WAIT_H
#define GENOS_WAIT_H

#include <sys/cdefs.h>
#include <igris/datastruct/dlist.h>

#include <genos/sched/schedee.h>

struct wait_head
{
	struct dlist_head wait_lnk;
};

struct wait_node
{
	struct dlist_head wait_lnk;
	void(*handler)(void*);
	void* handarg;
};

__BEGIN_DECLS

// Инициализировать очередь ожидания.
void wait_head_init(struct wait_head* w);

// Поставить текущий schedee в очередь и вызвать displace.
int wait_current_schedee(struct wait_head * head, int priority);

// Освободить первый узел очереди.
void unwait_one(struct wait_head * head);

// Освободить все узлы очереди.
void unwait_all(struct wait_head * head);

__END_DECLS

#endif