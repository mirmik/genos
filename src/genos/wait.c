#include <genos/wait.h>

static int waiter_to_list_and_displace(struct wait_head * head, int priority, 
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

int wait_current_schedee(struct wait_head * head, int priority) 
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

void unwait_one(struct wait_head * head)
{
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

void unwait_all(struct wait_head * head)
{
	struct waiter * it;

	system_lock();

	dlist_for_each_entry(it, head, wait_lnk) {
		__unwait(it);

	system_unlock();
	} 
}