#include <genos/wait.h>
#include <genos/schedee.h>
#include <genos/schedee_api.h>

#include <igris/util/macro.h>
#include <igris/sync/syslock.h>

int unwait_schedee_waiter(struct waiter* w) 
{
	struct schedee * sch = mcast_out(w, struct schedee, waiter);
	sch->start();

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

	irqstate_t save = irqs_save();
	dlist_del_init(&sch->ctr.lnk);

	if (priority) 
		dlist_move(&sch->ctr.lnk, head);
	else 
		dlist_move_tail(&sch->ctr.lnk, head);

	irqs_restore(save);

	return __displace__();
}

int waitchild() 
{
	struct schedee * sch;
	sch = current_schedee();

	sch->sch_state = SCHEDEE_STATE_WAIT_SCHEDEE;

	system_lock();
	dlist_del_init(&sch->ctr.lnk);
	system_unlock();

	__displace__();

	return 0;
}