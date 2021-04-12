#ifndef GENOS_TIMER_TASKLET_H
#define GENOS_TIMER_TASKLET_H

#include <igris/sync/syslock.h>
#include <igris/osinter/ctrobj.h>
#include <systime/systime.h>

extern struct dlist_head ktimer_list;

namespace genos 
{
	struct ktimer;
}

typedef void(* ktimer_callback_t)(void* arg, genos::ktimer * tim);

struct ktimer_base
{
	struct ctrobj ctr;

	int64_t start;
	int64_t interval;

	ktimer_base(int64_t start, int64_t interval) : 
			ctr(CTROBJ_DECLARE(ctr, CTROBJ_KTIMER_DELEGATE)),
			start(start), 
			interval(interval)
	{}

	void plan();

	void swift()
	{
		start += interval;
	}

	void replan()
	{
		swift();
		plan();
	}

	bool planned();
	void unplan();	

	void set_start_now();
	void set_interval_ms(int64_t t);
};

namespace genos
{
	struct ktimer 
	{
		struct ktimer_base tim;

		ktimer_callback_t act;
		void * arg;

		ktimer(ktimer_callback_t act, void* arg, int64_t interval) : 
			tim(0, interval),
			act(act),
			arg(arg)
		{}

		void plan() { tim.plan(); }
		void replan() { tim.replan(); }
	};
}

__BEGIN_DECLS

void ktimer_manager_step();

void ktimer_init_for_milliseconds(struct ktimer * tim, ktimer_callback_t act, void* arg,
                                  uint32_t ms);
void ktimer_base_init_for_milliseconds(struct ktimer_base * tim, uint32_t interval, uint8_t ctrtype);

void ktimer_list_debug_print();

__END_DECLS

#endif