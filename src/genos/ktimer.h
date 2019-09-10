#ifndef GENOS_TIMER_TASKLET_H
#define GENOS_TIMER_TASKLET_H

#include <igris/sync/syslock.h>
#include <igris/osinter/ctrobj.h>

#include <systime/systime.h>

namespace genos 
{
	struct ktimer;
}

typedef void(* ktimer_callback_t)(void* arg, genos::ktimer * tim);

struct ktimer_base
{
	struct ctrobj ctr;

	clock_t start;
	clock_t interval;

	ktimer_base(clock_t start, clock_t interval) : 
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
	void set_interval_ms(clock_t t);
};

namespace genos
{
	struct ktimer : public ktimer_base
	{
		ktimer_callback_t act;
		void * arg;

		ktimer(ktimer_callback_t act, void* arg, clock_t interval) : 
			ktimer_base(0, interval),
			act(act),
			arg(arg)
		{}
	};
}

/*#define KTIMER_DECLARE(name, act, arg, interval) 				\
struct ktimer name = { 											\
	{ 															\
		CTROBJ_DECLARE(name.tim.ctr, CTROBJ_KTIMER_DELEGATE), 	\
		0, 														\
		interval 												\
	}, 															\
	act, 														\
	arg 														\
}*/

__BEGIN_DECLS

void ktimer_manager_step();

void ktimer_init_for_milliseconds(struct ktimer * tim, ktimer_callback_t act, void* arg,
                                  uint32_t ms);
void ktimer_base_init_for_milliseconds(struct ktimer_base * tim, uint32_t interval, uint8_t ctrtype);



void ktimer_list_debug_print();

//void ktimer_base_plan(struct ktimer_base * t);

// Сместить временную отметку начала отсчета с тем, чтобы обеспечить
// срабатывание таймера через равные периоды времени.
//void ktimer_swift(struct ktimer * t);

//static inline
//void ktimer_plan(struct ktimer * t)
//{
//	ktimer_base_plan(&t->tim);
//}

// Сместить временную отметку и перепланировать таймер.
//static inline
//void ktimer_replan(struct ktimer * t)
//{
///	ktimer_swift(t);
//	ktimer_plan(t);
//}

__END_DECLS

#endif