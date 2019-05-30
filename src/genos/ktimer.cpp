#include <stdlib.h>
#include <igris/dprint.h>

#include <genos/schedee.h>
#include <genos/sched.h>

#include <genos/ktimer.h>

#include <assert.h>

DLIST_HEAD(ktimer_list);

void ktimer_base_init(struct ktimer_base * tim, clock_t start, clock_t interval, uint8_t ctrtype)
{
	ctrobj_init(&tim->ctr, ctrtype);
	tim->start = start;
	tim->interval = interval;
}

void ktimer_init(struct ktimer * tim, ktimer_callback_t act, void* arg, clock_t start, clock_t interval)
{
	ktimer_base_init(&tim->tim, start, interval, CTROBJ_KTIMER_DELEGATE);
	tim->act = act;
	tim->arg = arg;
}

static inline 
uint8_t ktimer_check(struct ktimer_base * tim, clock_t now)
{
	return now - tim->start >= tim->interval;
}

void ktimer_swift(struct ktimer * tim)
{
	tim->tim.start += tim->tim.interval;
}


void ktimer_base_init_for_milliseconds(struct ktimer_base * tim, uint32_t interval, uint8_t ctrtype)
{
	ktimer_base_init(tim, jiffies(), ms2jiffies(interval), ctrtype);
}

void ktimer_init_for_milliseconds(struct ktimer * tim, ktimer_callback_t act, void* arg, uint32_t interval)
{
	ktimer_init(tim, act, arg, jiffies(), ms2jiffies(interval));
}



void ktimer_base_plan(struct ktimer_base * t)
{
	struct ktimer_base * it;
	struct dlist_head * sit;
	clock_t it_final;
	clock_t final;

	final = t->start + t->interval;
	sit = NULL;

	system_lock();

	dlist_for_each_entry(it, &ktimer_list, ctr.lnk)
	{
		it_final = it->start + it->interval;

		if (final - it_final < 0)
		{
			sit = &it->ctr.lnk;
			break;
		}
	}

	dlist_add_tail(&t->ctr.lnk, sit ? sit : &ktimer_list);

	system_unlock();
}


void ktimer_execute(struct ktimer_base * tim)
{
	switch (tim->ctr.type)
	{
		case CTROBJ_KTIMER_DELEGATE: 
		{
			struct ktimer * t = mcast_out(tim, struct ktimer, tim);
			dlist_del(&tim->ctr.lnk);
			t->act(t->arg, t);
			break;
		}

		case CTROBJ_KTIMER_SCHEDEE: 
		{
			struct schedee * sch = mcast_out(tim, struct schedee, ktimer);
			// извлечение из списка произойдёт при запуске, т.к. waiter использует поле schedee листа.
			schedee_run(sch);
			break;
		}

		default:
			break;
	}
}


void ktimer_manager_step()
{
	clock_t now;

	now = jiffies();

	system_lock();

	while (!dlist_empty(&ktimer_list))
	{
		struct ktimer_base * it = dlist_first_entry(&ktimer_list, struct ktimer_base, ctr.lnk);
		system_unlock();

		if (ktimer_check(it, now))
		{
			ktimer_execute(it);
		}
		else break;

		system_lock();
	}

	system_unlock();
}