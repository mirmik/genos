#include <genos/wait.h>
#include <sched/sched.h>

#include <igris/util/macro.h>
#include <igris/sync/syslock.h>

int waiter_unwait(struct dlist_head * lnk)
{
	struct ctrobj * ctr = mcast_out(lnk, struct ctrobj, lnk);

	switch (ctr->type)
	{
		case CTROBJ_WAITER_SCHEDEE:
			{
				struct waiter * w = mcast_out(ctr, struct waiter, ctr);
				struct schedee * sch = mcast_out(w, struct schedee, waiter);
				// извлечение из списка произойдёт при запуске, т.к. waiter использует поле schedee листа.
				schedee_run(sch);
			}
			return 0;

		case CTROBJ_WAITER_DELEGATE:
			{
				struct waiter_delegate * w = mcast_out(ctr, struct waiter_delegate, ctr);
				dlist_del(lnk);
				w->func(w->obj);
			}
			return 0;

		default:
			return -1;
	}
}

int wait_current_schedee(struct dlist_head * head, int priority) 
{
	struct schedee * sch;

	sch = current_schedee();

	if (sch == NULL || !sch->flag.can_displace)
		return -1;

	sch->ctr.type = CTROBJ_WAITER_SCHEDEE;
	sch->state = SCHEDEE_STATE_WAIT;

	system_lock();

	if (priority) 
		dlist_move(&sch->ctr.lnk, head);
	else 
		dlist_move_tail(&sch->ctr.lnk, head);

	system_unlock();

	return __displace__();
}

void unwait_one(struct dlist_head * head)
{
	struct dlist_head * it;

	system_lock();

	if (dlist_empty(head)) {
		system_unlock();
		return;
	}
	
	it = head->next;
	waiter_unwait(it);

	system_unlock();
}

void unwait_all(struct dlist_head * head)
{
	struct dlist_head * it;

	system_lock();

	dlist_for_each(it, head) 
	{
		waiter_unwait(it);
	}

	system_unlock();
}