#ifndef GENOS_SLINK_HEAD_H
#define GENOS_SLINK_HEAD_H

#include "defines/decltypeof.h"
//#include "kernel/kassert.h"
#include "util/member.h"
#include "inttypes.h"
struct slist_head
{
	struct slist_head* next;
};

void slist_init(struct slist_head* link)
{
	//kassert(link);
	link->next = link;
}

void slist_connect_after(struct slist_head* link, struct slist_head* head)
{
	//kassert(link);
	//kassert(head);
	link->next = head->next;
	head->next = link;
};

void slist_connect_last(struct slist_head* link, struct slist_head* head)
{
	//kassert(link);
	//kassert(head);
	struct slist_head* iter = head;
	while(iter->next != iter) iter = iter->next;
	slist_connect_after(link, iter);
};

static inline int8_t slist_is_empty(struct slist_head* head)
{
	return head == head->next;
};

#define slist_entry(ptr, type, member) \
container_of(ptr, type, member)

#define slist_next_entry(pos, member) \
slist_entry((pos)->member.next, decltypeof(*(pos)), member)

#define slist_first_entry(ptr, type, member) \
slist_entry((ptr)->next, type, member)

#define slist_for_each(pos, head) \
for (pos = (head)->next; pos != (head); pos = pos->next)

#define slist_for_each_safe(pos, n, head) \
for (pos = (head)->next, n = pos->next; pos != (head); \
pos = n, n = pos->next)

#define slist_for_each_entry(pos, head, member)				\
for (pos = slist_first_entry(head, decltypeof(*pos), member);	\
&pos->member != (head);					\
pos = slist_next_entry(pos, member))

#define slist_for_each_entry_safe(pos, n, head, member)			\
for (pos = slist_first_entry(head, decltypeof(*pos), member),	\
n = slist_next_entry(pos, member);			\
&pos->member != (head); 					\
pos = n, n = slist_next_entry(n, member))
#endif