#include <stdlib.h>
#include <igris/dprint.h>
#include <igris/time/jiffies-systime.h>
#include <genos/schedee.h>
#include <genos/ktimer.h>

DLIST_HEAD(ktimer_list);

void ktimer_base_init(
	struct ktimer_base * tim, 
	int64_t start, 
	int64_t interval, 
	uint8_t ctrtype)
{
	ctrobj_init(&tim->ctr, ctrtype);
	tim->start = start;
	tim->interval = interval;
}

void ktimer_init(genos::ktimer * tim, ktimer_callback_t act, void* arg, int64_t start, int64_t interval)
{
	ktimer_base_init(&tim->tim, start, interval, CTROBJ_KTIMER_DELEGATE);
	tim->act = act;
	tim->arg = arg;
}

static inline 
uint8_t ktimer_check(struct ktimer_base * tim, int64_t now)
{
	return now - tim->start >= tim->interval;
}

void ktimer_base_init_for_milliseconds(struct ktimer_base * tim, uint32_t interval, uint8_t ctrtype)
{
	ktimer_base_init(tim, jiffies(), ms2jiffies(interval), ctrtype);
}

void ktimer_init_for_milliseconds(genos::ktimer * tim, ktimer_callback_t act, void* arg, uint32_t interval)
{
	ktimer_init(tim, act, arg, jiffies(), ms2jiffies(interval));
}

void ktimer_base::set_start_now() { start = jiffies(); }
void ktimer_base::set_interval_ms(int64_t t) { interval = ms2jiffies(t); }

void ktimer_list_debug_print() 
{
	struct ktimer_base * it;
	dlist_for_each_entry(it, &ktimer_list, ctr.lnk) 
	{
		dprln("timer", it->start, it->interval);
	}
}

bool ktimer_base::planned() 
{
	return ctr.lnk.next != ctr.lnk.prev;
}

void ktimer_base::unplan() 
{
	dlist_del_init(&ctr.lnk);
}

void ktimer_base::plan()
{
	struct ktimer_base * it;
	struct dlist_head * sit = nullptr;
	int64_t it_final;
	int64_t final;

	final = start + interval;
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

	dlist_add_tail(&ctr.lnk, sit ? sit : &ktimer_list);

	system_unlock();
}


void ktimer_execute(struct ktimer_base * tim)
{
	switch (tim->ctr.type)
	{
		case CTROBJ_KTIMER_DELEGATE: 
		{
			genos::ktimer * t = mcast_out(tim, genos::ktimer, tim);
			dlist_del_init(&tim->ctr.lnk);
			t->act(t->arg, t);
			break;
		}

		case CTROBJ_KTIMER_SCHEDEE: 
		{
			genos::schedee * sch = mcast_out(tim, genos::schedee, ktimer);
			// извлечение из списка произойдёт при запуске, т.к. waiter использует поле schedee листа.
			schedee_start(sch);
			break;
		}

		default:
			break;
	}
}


void ktimer_manager_step()
{
	int64_t now;

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
		else return;

		system_lock();
	}

	system_unlock();
}
