#include <genos/schedee_api.h>
#include <genos/schedee.h>
#include <genos/ktimer.h>
#include <genos/displace.h>

#include <igris/sync/syslock.h>
#include <igris/util/bug.h>

using namespace genos;

void genos::current_schedee_exit()
{
	genos::schedee * sch;

	sch = current_schedee();
	__schedee_final(sch);

	__displace__();
}

int genos::current_schedee_displace()
{
	genos::schedee * sch = current_schedee();

	if (sch->u.f.can_displace == 0) 
	{
		return -1;
	}

	sch->syslock_counter_save = syslock_counter();

//#if SCHEDEE_DEBUG_STRUCT
//	++sch->dispcounter;
//#endif

	return sch->displace();
}

void __timer_schedee_unsleep(void * priv, genos::ktimer * tim) 
{
	(void) tim;
	genos::schedee * sch = (genos::schedee *) priv;
	schedee_start(sch);
}

int genos::current_schedee_msleep(unsigned int ms, int64_t start)
{
	genos::schedee * sch;
	genos::ktimer * timer;

	sch = current_schedee();

	if (sch == NULL)
	{
		return -1;
	}

	timer = &sch->ktimer;

	system_lock();
	dlist_del_init(&sch->ctr.lnk);
	system_unlock();

	sch->ctr.type = CTROBJ_KTIMER_SCHEDEE;
	sch->sch_state = SCHEDEE_STATE_WAIT;

	//ktimer_init(
	*timer = genos::ktimer( 
		__timer_schedee_unsleep, 
		(void*) sch, 
		start,    //start 
		ms                  //interval
	);

	timer->plan();

	int re = current_schedee_displace();
	return re;
}

int genos::current_schedee_msleep(unsigned int ms) 
{
	return genos::current_schedee_msleep(ms, igris::system_time());
}