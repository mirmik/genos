#include <genos/schedee_api.h>
#include <genos/schedee.h>
#include <genos/ktimer.h>
#include <genos/displace.h>

#include <igris/sync/syslock.h>
#include <igris/util/bug.h>

void current_schedee_exit()
{
	struct schedee * sch;

	sch = current_schedee();
	__schedee_final(sch);

	__displace__();
}

int current_schedee_displace()
{
	struct schedee * sch = current_schedee();

	if (sch->flag.can_displace == 0) 
	{
		return -1;
	}

	sch->syslock_counter_save = syslock_counter();

#if SCHEDEE_DEBUG_STRUCT
	++sch->dispcounter;
#endif

	return sch->ops->displace(sch);
}

void __timer_schedee_unsleep(void * priv, struct ktimer_head * tim) 
{
	(void) tim;
	struct schedee * sch = (struct schedee *) priv;
	schedee_start(sch);
}

int current_schedee_msleep(unsigned int ms)
{
	struct schedee * sch;
	struct ktimer_head * timer;

	sch = current_schedee();

	if (sch == NULL)
	{
		BUG();
	}

	timer = &sch->ktimer;

	system_lock();
	dlist_del_init(&sch->ctr.lnk);
	system_unlock();

	sch->ctr.type = CTROBJ_KTIMER_SCHEDEE;
	sch->sch_state = SCHEDEE_STATE_WAIT;

	ktimer_init(
		timer, 
		__timer_schedee_unsleep, 
		(void*) sch, 
		millis(),           //start 
		ms                  //interval
	);

	ktimer_plan(timer);

	int re = current_schedee_displace();
	return re;
}