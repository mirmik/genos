#include <sched/wait.h>
#include <sched/schedee.h>
#include <sched/sched.h>

#include <igris/sync/syslock.h>

#include <stdlib.h>

#include <igris/datastruct/dlist.h>
#include <igris/util/bug.h>

#define NODTRACE 1
#include <igris/dtrace.h>

static int __to_list_and_displace(struct dlist_head * head, int priority, 
	struct dlist_head * wp
) {
	DTRACE();

	system_lock();

	dlist_del(wp);

	if (priority) 
		dlist_add(wp, head);
	else 
		dlist_add_tail(wp, head);

	system_unlock();

	return __displace__();
}

int wait_current_schedee(struct dlist_head * head, int priority) 
{
	DTRACE();
	struct schedee * cur;

	cur = current_schedee();

	if (cur == NULL || !cur->flag.can_displace)
		return -1;

	schedee_wait(cur);
	return __to_list_and_displace(head, priority, &cur->lnk);
}

extern struct dlist_head runlist[];
void __unwait(struct dlist_head * w)
{
	DTRACE();
	system_lock();
	dlist_del_init(w);

	//dlist_debug_print(&runlist[0]);
	//DPRINT(dlist_size(&runlist[0]));
	//DPRINT(dlist_oposite_size(&runlist[0]));

	struct schedee * sch = mcast_out(w, struct schedee, lnk);
	schedee_run(sch);

	//dlist_debug_print(&runlist[0]);

	system_unlock();
}

void unwait_one(struct dlist_head * head)
{
	DTRACE();
	struct dlist_head * it;

	system_lock();

	if (dlist_empty(head)) {
		system_unlock();
		return;
	}
	
	it = head->next;
	__unwait(it);

	system_unlock();
}

void unwait_all(struct dlist_head * head)
{
	DTRACE();
	struct dlist_head * it;

	system_lock();

	dlist_for_each(it, head) 
	{
		__unwait(it);
	}

	system_unlock();
}

/*struct waiter 
{
	struct dlist_head wait_lnk;
	void(*handler)(void*);
	void* handarg;
	bool dynamic;
};

void init_waiter(struct waiter * w, void(*handler)(void*), void * handarg)
{
	w->handarg = handarg;
	w->handler = handler;
}*/

/*struct waiter * waiter_get(void(*handler)(void*), void* handarg) 
{
	struct waiter * w = malloc(sizeof(struct waiter));
	init_waiter(w, handler, handarg);
	w->dynamic = true;
	return w;
}

void waiter_put(struct waiter * w) 
{
	free(w);
}*/

/*static int waiter_to_list(struct dlist_head * head, int priority, 
	struct waiter * wp
) {
	system_lock();

	if (priority) 
		dlist_add(&wp->wait_lnk, head);
	else 
		dlist_add_tail(&wp->wait_lnk, head);

	system_unlock();

	return 0;
}*/

/*static int waiter_to_list_and_displace(struct dlist_head * head, int priority, 
	struct waiter * wp
) {
	system_lock();

	if (priority) 
		dlist_add(&wp->wait_lnk, head);
	else 
		dlist_add_tail(&wp->wait_lnk, head);

	system_unlock();
	//	dprln("Displace");

	return __displace__();
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


	if (!cur->flag.has_context) 
	{
		//BUG();
		struct waiter * w = waiter_get(__unwait_schedee_by_selflist, (void*)cur);
		schedee_wait(cur);
		return waiter_to_list_and_displace(head, priority, w);
	}
	else 
	{
	//	dprln("DOWAITER");
		struct waiter w;
		schedee_wait(cur);
		return waiter_to_list_and_displace(head, priority, &w);
	}
}

void __unwait(struct waiter * w)
{
	void *handarg 			= w->handarg;
	void(*handler)(void*) 	= w->handler;
	
	system_lock();
	dlist_del(&w->wait_lnk);
	system_unlock();
	
	//if (w->dynamic)
	//	waiter_put(w);

	handler( handarg );
}

void unwait_one(struct dlist_head * head)
{
	//dprln("unwait");
	struct waiter * it;

	system_lock();

	if (dlist_empty(head)) {
		system_unlock();
		return;
	}
	
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

void __unwait_flag(void* priv) 
{
	volatile uint8_t * flag = (volatile uint8_t *) priv;
	*flag = 1;
}

/*int wait_flag_setted(struct dlist_head * head, int priority) 
{
	//struct schedee * cur;
	volatile uint8_t flag = 0;

//	struct waiter * w = waiter_get(__unwait_flag, (void*)&flag);
//	waiter_to_list(head, priority, w);

	if (!cur->flag.has_context) 
	{
		BUG();
	//	struct waiter * w = waiter_get(__unwait_schedee, (void*)cur);
	//	schedee_wait(cur);
	//	return waiter_to_list_and_displace(head, priority, w);
	}
	else 
	{
		struct waiter w;
		schedee_wait(cur);
		return waiter_to_list_and_displace(head, priority, &w);
	}

	while(flag == 0);
	return 0;
}*/