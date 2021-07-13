#include <genos/wait.h>
#include <genos/schedee.h>
#include <genos/schedee_api.h>

#include <igris/util/macro.h>
#include <igris/sync/syslock.h>

int unwait_schedee_waiter(struct waiter* w) 
{
	struct schedee * sch = mcast_out(w, struct schedee, waiter);
	schedee_start(sch);

	return 0;
}

int wait_current_schedee(struct dlist_head * head, int priority, void** future) 
{
	struct schedee * sch;

	sch = current_schedee();

	if (sch == NULL || !sch->flag.can_displace)
		return -1;

	sch->ctr.type = CTROBJ_WAITER_SCHEDEE;
	sch->sch_state = SCHEDEE_STATE_WAIT;

	system_lock();
	dlist_del_init(&sch->ctr.lnk);

	if (priority) 
		dlist_move(&sch->ctr.lnk, head);
	else 
		dlist_move_tail(&sch->ctr.lnk, head);

	system_unlock();

	return current_schedee_displace();
}

int waitchild() 
{
	struct schedee * sch;
	sch = current_schedee();

	sch->sch_state = SCHEDEE_STATE_WAIT_SCHEDEE;

	system_lock();
	dlist_del_init(&sch->ctr.lnk);
	system_unlock();

	current_schedee_displace();

	return 0;
}