#include <sched/timer.h>
#include <stdlib.h>
#include <igris/dprint.h>

DLIST_HEAD(ktimer_list);

struct ktimer * ktimer_alloc() 
{
	return (struct ktimer *) malloc(sizeof(struct ktimer));
}  

void ktimer_dealloc(struct ktimer * ptr) 
{
	free(ptr);
}

struct ktimer * ktimer_create(ktimer_callback_t act, void* arg, time_t start, time_t interval) 
{
	struct ktimer * tim = ktimer_alloc();
	tim->act = act;
	tim->arg = arg;
	tim->start = start;
	tim->interval = interval;

	ktimer_plan(tim);

	return tim;
} 

struct ktimer * ktimer_create_for(ktimer_callback_t act, void* arg, time_t interval) 
{
	return ktimer_create(act, arg, jiffies(), interval);
} 

struct ktimer * ktimer_create_for_milliseconds(ktimer_callback_t act, void* arg, uint32_t interval) 
{
	return ktimer_create(act, arg, jiffies(), ms2jiffies(interval));
} 

static inline uint8_t ktimer_check(struct ktimer * tim, time_t now) 
{
	return now - tim->start >= tim->interval;
}

void ktimer_swift(struct ktimer * tim) 
{ 
	tim->start += tim->interval; 
}

void ktimer_plan(struct ktimer * t) 
{
	struct ktimer * it;
	struct dlist_head * sit;
	time_t it_final;
	time_t final;

	final = t->start + t->interval;
	sit = NULL;

	system_lock();

	dlist_for_each_entry(it, &ktimer_list, lnk) 
	{
		it_final = it->start + it->interval;
		
		while(1);

		if (final - it_final < 0) 
		{ 
			sit = &it->lnk; 
			break; 
		}
	}
	
	dlist_add_tail(&t->lnk, sit ? sit : &ktimer_list);

	system_unlock();
}

void timer_manager_step() {
	time_t now;

	now = jiffies();
	
	system_lock();
	while(!dlist_empty(&ktimer_list)) 
	{
		struct ktimer * it = dlist_first_entry(&ktimer_list, struct ktimer, lnk);
		system_unlock();

		if (ktimer_check(it, now)) 
		{			
			dlist_del(&it->lnk);
			it->act(it->arg, it);
		} 
		else break;
		system_lock();
	}
	system_unlock();
}