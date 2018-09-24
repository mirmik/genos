#include <sched/wait.h>
#include <sched/schedee.h>

struct schedee_waiter {
	struct dlist_head wait_lnk;
	struct dlist_head proc_lnk;
	struct schedee * sch;
};

void init_waiter(struct waiter * wait, struct schedee * sch) {
	//dlist_add(wait->proc_lnk)
	wait->sch = sch;
}

static int __wait_current_schedee(struct dlist_head * head, bool priority, struct waiter * wp) 
{
	if (priority) 
		dlist_add(&wp->wait_lnk, head);
	else 
		dlist_add_tail(&wp->wait_lnk, head);

	__displace__();
	return 0;
}

int wait_current_schedee(struct dlist_head * head, bool priority) 
{
	struct schedee * cur;

	cur = current_schedee();

	if (cur == NULL || !schedee_can_displace(cur))
		return -1;
	
	if (schedee_has_context(cur)) {
		struct schedee_waiter w;
		return __wait_current_schedee(head, priority, &w);
	} else {
		struct schedee_waiter * wp = (struct schedee_waiter *) 
			malloc(sizeof(struct schedee_waiter));
		return __wait_current_schedee(head, priority, wp);
	}
}

int unwait_one(struct dlist_head * head) {

}

int unwait_all(struct dlist_head * head) {

}