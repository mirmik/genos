#include <genos/api.h>
#include <genos/ktimer.h>

//#include <igris/dprint.h>

int msleep(unsigned int ms)
{
	struct schedee * sch;
	struct ktimer_base * timer;

	sch = current_schedee();

	if (sch == NULL || !sch->flag.can_displace)
	{
		delay(ms);
		return 0;
	}
	
	timer = &sch->ktimer;

	system_lock();
	dlist_del(&sch->ctr.lnk);
	system_unlock();

	sch->ctr.type = CTROBJ_KTIMER_SCHEDEE;
	sch->state = SCHEDEE_STATE_WAIT;

	ktimer_base_init_for_milliseconds(timer, ms, CTROBJ_KTIMER_SCHEDEE);
	ktimer_base_plan(timer);

	return __displace__();
}