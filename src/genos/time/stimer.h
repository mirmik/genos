#ifndef GENOS_SCHED_SIMPLEST_TIME_MANAGER_H
#define GENOS_SCHED_SIMPLEST_TIME_MANAGER_H

#include <sys/cdefs.h>
#include <stdbool.h>

typedef struct stimer_s {
	dlist_head lnk;
	uint32_t start;
	uint32_t period;

	void(*func)(void*);
	void* privdata;
} stimer_t;

typedef struct timemngr_s {
	dlist_head list;
} timemngr_t;

__BEGIN_DECLS

void stimer_init(stimer_t* timer, uint32_t start, uint32_t period, void(*func)(void*), void* privdata);
bool stimer_check(stimer_t* timer, uint32_t current_ms);
uint32_t stimer_remainder(stimer_t* timer, uint32_t current_ms);
void stimer_doit(stimer_t* timer);

__END_DECLS

#endif