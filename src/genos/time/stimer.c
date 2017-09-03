#include <genos/time/stimer.h>

stimer_manager_t timemngr;

void stimer_manager_init() {
	dlist_init(&timemngr->list);
}

void stimer_init(stimer_t* timer, uint32_t start, uint32_t period, void(*func)(void*), void* privdata) {
	timer->start = start;
	timer->period = period;
	timer->func = func;
	timer->privdata = privdata;
}

bool stimer_check(stimer_t* timer, uint32_t current_ms) {
	return ms - timer->start >= period;
}

uint32_t stimer_remainder(stimer_t* timer, uint32_t current_ms) {
	return period - ms + timer->start;
}

void stimer_doit(stimer_t* timer) {
	timer->func(timer->privdata);
}