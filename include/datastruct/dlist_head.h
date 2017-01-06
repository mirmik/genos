#ifndef GENOS_DLIST_HEAD
#define GENOS_DLIST_HEAD

#include "inttypes.h"
#include "assert.h"
#include "defines/decltypeof.h"
#include "util/member.h"
#include "compiler.h"

struct dlist_head
{
	struct dlist_head* next;
	struct dlist_head* prev;

	__if_cplusplus(
	inline dlist_head() : next(this), prev(this) {};
	)
};

__BEGIN_DECLS

static inline void dlist_init(struct dlist_head* head)
{
	head->next = head;
	head->prev = head;
};

static inline void dlist_init_list(struct dlist_head* head)
{
	head->next = head;
	head->prev = head;
};

static inline int8_t dlist_is_empty(struct dlist_head* head)
{
	return head == head->next;
};
#define dlist_empty(arg) dlist_is_empty(arg)

static inline void __dlist_add(struct dlist_head *_new, struct dlist_head *next, struct dlist_head *prev) 
{
	_new->prev = prev;
	_new->next = next;
	next->prev = _new;
	prev->next = _new;
}

static inline void dlist_add_next(struct dlist_head* _new, struct dlist_head* head)
{
	__dlist_add(_new, head->next, head);
};
#define dlist_add_front(a,b) dlist_add_next(a,b)
#define dlist_add(a,b) dlist_add_next(a,b)

static inline void dlist_add_prev(struct dlist_head* _new, struct dlist_head* head)
{
	__dlist_add(_new, head, head->prev);
};
#define dlist_add_back(a,b) dlist_add_prev(a,b)
#define dlist_add_tail(a,b) dlist_add_prev(a,b)

static inline void __dlist_del(struct dlist_head* head)
{
	head->prev->next = head->next;
	head->next->prev = head->prev;
};

static inline void dlist_del(struct dlist_head* head)
{
	__dlist_del(head);
	dlist_init(head);	
};
#define dlist_del_init(x) dlist_del(x)

static inline void dlist_move_next(struct dlist_head* lst, struct dlist_head* head)
{
	__dlist_del(lst);
	dlist_add_next(lst, head);
};

static inline void dlist_move_prev(struct dlist_head* lst, struct dlist_head* head)
{
	__dlist_del(lst);
	dlist_add_prev(lst, head);
};

__END_DECLS

#define dlist_entry(ptr, type, member) \
container_of(ptr, type, member)

#define dlist_next_entry(pos, member) \
dlist_entry((pos)->member.next, decltypeof(*(pos)), member)

#define dlist_prev_entry(pos, member) \
dlist_entry((pos)->member.prev, decltypeof(*(pos)), member)

#define dlist_first_entry(ptr, type, member) \
dlist_entry((ptr)->next, type, member)

#define dlist_last_entry(ptr, type, member) \
dlist_entry((ptr)->prev, type, member)

#define dlist_for_each(pos, head) \
for (pos = (head)->next; pos != (head); pos = pos->next)

#define dlist_for_each_safe(pos, n, head) \
for (pos = (head)->next, n = pos->next; pos != (head); \
pos = n, n = pos->next)

#define dlist_for_each_entry(pos, head, member)				\
for (pos = dlist_first_entry(head, decltypeof(*pos), member);	\
&pos->member != (head);					\
pos = dlist_next_entry(pos, member))

#define dlist_for_each_entry_safe(pos, n, head, member)			\
for (pos = dlist_first_entry(head, decltypeof(*pos), member),	\
n = dlist_next_entry(pos, member);			\
&pos->member != (head); 					\
pos = n, n = dlist_next_entry(n, member))
	
#endif