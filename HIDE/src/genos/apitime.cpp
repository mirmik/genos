#include <genos/api.h>
#include <genos/ktimer.h>

#include <igris/util/bug.h>

int msleep(unsigned int ms)
{
	genos::schedee * sch;
	struct ktimer_base * timer;

	sch = current_schedee();

	if (sch == NULL || !sch->flag.can_displace)
	{
		BUG();
	}

	timer = &sch->ktimer;

	system_lock();
	dlist_del_init(&sch->ctr.lnk);
	system_unlock();

	sch->ctr.type = CTROBJ_KTIMER_SCHEDEE;
	sch->sch_state = SCHEDEE_STATE_WAIT;

	ktimer_base_init_for_milliseconds(timer, ms, CTROBJ_KTIMER_SCHEDEE);
	timer->plan();

	return __displace__();
}