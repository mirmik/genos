#ifndef GENOS_WAIT_H
#define GENOS_WAIT_H

#include <genos/ctrobj.h>
#include <sys/cdefs.h>

#define WAIT_PRIORITY 1

struct waiter
{
	struct ctrobj ctr;
};

struct waiter_delegate
{
	struct ctrobj ctr;
	void (*func)(void*);
	void * obj;
};

__BEGIN_DECLS

static inline
void waiter_schedee_init(struct waiter * w) 
{
	ctrobj_init(&w->ctr, CTROBJ_WAITER_SCHEDEE);
}

static inline
void waiter_delegate_init(struct waiter_delegate * w, void (*func)(void*), void* obj) 
{
	ctrobj_init(&w->ctr, CTROBJ_WAITER_DELEGATE);
	w -> func = func;
	w -> obj = obj;
}

int waiter_unwait(struct dlist_head * lnk);
int wait_current_schedee(struct dlist_head * head, int priority);

void unwait_one(struct dlist_head * head);
void unwait_all(struct dlist_head * head);

__END_DECLS

#endif