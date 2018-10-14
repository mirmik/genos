#include <sched/wait.h>
#include <sched/schedee.h>
#include <sched/sched.h>

#include <gxx/syslock.h>

#include <stdlib.h>

struct waiter 
{
	struct dlist_head wait_lnk;
	void(*handler)(void*);
	void* handarg;
};

void init_waiter(struct waiter * w, void(*handler)(void*), void * handarg)
{
	w->handarg = handarg;
	w->handler = handler;
}

struct waiter * waiter_get(void(*handler)(void*), void* handarg) 
{
	struct waiter * w = malloc(sizeof(struct waiter));
	init_waiter(w, handler, handarg);
	return w;
}

void waiter_put(struct waiter * w) 
{
	free(w);
}

static int waiter_to_list_and_displace(struct dlist_head * head, int priority, 
	struct waiter * wp
) {
	system_lock();

	if (priority) 
		dlist_add(&wp->wait_lnk, head);
	else 
		dlist_add_tail(&wp->wait_lnk, head);

	system_unlock();

	__displace__();
	return 0;
}


void __unwait_schedee(void* priv) 
{
	struct schedee * sch = (struct schedee *) priv;
	schedee_run(sch);
}

int wait_current_schedee(struct dlist_head * head, int priority) 
{
	struct schedee * cur;

	cur = current_schedee();

	if (cur == NULL || !cur->flag.can_displace)
		return -1;

	struct waiter * w = waiter_get(__unwait_schedee, (void*)cur);
	schedee_wait(cur);

	waiter_to_list_and_displace(head, priority, w);
	return 0;
}

void __unwait(struct waiter * w)
{
	void *handarg 			= w->handarg;
	void(*handler)(void*) 	= w->handler;
	
	system_lock();
	dlist_del(&w->wait_lnk);
	system_unlock();
	
	waiter_put(w);

	handler( handarg );
}

void unwait_one(struct dlist_head * head)
{
	struct waiter * it;

	system_lock();

	if (dlist_empty(head)) 
		return;
	
	it = dlist_first_entry(head, struct waiter, wait_lnk);
	__unwait(it);

	system_unlock();
}

void unwait_all(struct dlist_head * head)
{
	struct waiter * it;

	system_lock();

	dlist_for_each_entry(it, head, wait_lnk) {
		__unwait(it);

	system_unlock();
	} 
}