#include <sched/timer.h>
#include <stdlib.h>
#include <gxx/debug/dprint.h>

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
	struct ktimer * it;// = dlist_first_entry(&ktimer_list, struct ktimer, lnk);
	struct dlist_head * sit = NULL;
	//struct ktimer * eit = dlist_last_entry(&ktimer_list, struct ktimer, lnk);
	
	time_t final = t->start + t->interval;
	time_t it_final;

	//for(; it != eit; it = dlist_next_entry(it, lnk)) 
	//{
	dlist_for_each_entry(it, &ktimer_list, lnk) 
	{
		it_final = it->start + it->interval;
		if (final - it_final < 0) 
		{ 
			sit = &it->lnk; 
			break; 
		}
	}
	//}
	
	dlist_add_tail(&t->lnk, sit ? sit : &ktimer_list);
}

void timer_manager() {
	time_t now = jiffies();
	
	while(!dlist_empty(&ktimer_list)) 
	{
		struct ktimer * it = dlist_first_entry(&ktimer_list, struct ktimer, lnk);
		
		if (ktimer_check(it, now)) 
		{			
			dlist_del(&it->lnk);
			it->act(it->arg, it);
		} 
		else break;
	}
}