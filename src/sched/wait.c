#include <sched/wait.h>
#include <sched/schedee.h>
#include <sched/sched.h>

#include <stdlib.h>

struct schedee_waiter 
{
	struct dlist_head wait_lnk;
	//struct dlist_head proc_lnk;
	struct schedee * sch;
};

void init_waiter(struct schedee_waiter * wait, struct schedee * sch)
{
	wait->sch = sch;
}

static int __wait_current_schedee(struct dlist_head * head, int priority, 
	struct schedee_waiter * wp
) {
	if (priority) 
		dlist_add(&wp->wait_lnk, head);
	else 
		dlist_add_tail(&wp->wait_lnk, head);

	__displace__();
	return 0;
}

int wait_current_schedee(struct dlist_head * head, int priority) 
{
	struct schedee * cur;

	cur = current_schedee();

	if (cur == NULL || !cur->flag.can_displace)
		return -1;
	
	if (cur->flag.has_context) {
		struct schedee_waiter w;
		return __wait_current_schedee(head, priority, &w);
	} else {
		struct schedee_waiter * wp = (struct schedee_waiter *) 
			malloc(sizeof(struct schedee_waiter));
		return __wait_current_schedee(head, priority, wp);
	}
}

void __unwait(struct schedee_waiter * it)
{
	struct schedee * sch = it->sch;
	
	dlist_del(&it->wait_lnk);
	
	if (sch->flag.has_context)
		free(it);

	schedee_run(sch);
}

void unwait_one(struct dlist_head * head)
{
	struct schedee_waiter * it;
	it = dlist_first_entry(head, struct schedee_waiter, wait_lnk);
	__unwait(it);
}

void unwait_all(struct dlist_head * head)
{
	struct schedee_waiter * it;
	dlist_for_each_entry(it, head, wait_lnk) {
		__unwait(it);
	} 
}