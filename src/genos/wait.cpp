#include <genos/wait.h>
#include <genos/sched.h>
#include <genos/schedee.h>

#include <igris/util/macro.h>
#include <igris/sync/syslock.h>

int unwait_schedee_waiter(struct waiter* w) 
{
	genos::schedee * sch = mcast_out(w, genos::schedee, waiter);
	sch->start();

	return 0;
}

int wait_current_schedee(struct dlist_head * head, int priority, void** future) 
{
	genos::schedee * sch;

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
