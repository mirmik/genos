#ifndef GENOS_TIMER_TASKLET_H
#define GENOS_TIMER_TASKLET_H

#include <igris/datastruct/dlist.h>
#include <igris/sync/syslock.h>
#include <systime/systime.h>

struct ktimer;

typedef void(* ktimer_callback_t)(void* arg, struct ktimer * tim);

struct ktimer {
	struct dlist_head lnk;
	ktimer_callback_t act;
	void * arg;
	time_t start;
	time_t interval;
};


__BEGIN_DECLS

void timer_manager();

struct ktimer * ktimer_create(ktimer_callback_t act, void* arg, 
	time_t start, time_t interval);

struct ktimer * ktimer_create_for(ktimer_callback_t act, void* arg, 
	time_t interval);

struct ktimer * ktimer_create_for_milliseconds(ktimer_callback_t act, void* arg, 
	uint32_t ms);

void ktimer_dealloc(struct ktimer * ptr);

void ktimer_plan(struct ktimer * t);
void ktimer_swift(struct ktimer * t);

__END_DECLS

#endif