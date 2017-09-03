#ifndef GENOS_AUTOM_SCHEDEE_H
#define GENOS_AUTOM_SCHEDEE_H

#include <genos/sched/schedee.h>
#include <gxx/util/stub.h>

typedef void(*autom_func_t)(void*);

typedef struct autom_schedee_s {
	schedee_t sch;
	void* privdata;
	autom_func_t func;
} autom_schedee_t;

//DEBUG: Имплементация schedee для тестирования shedee_manager
static inline void autom_schedee_execute(schedee_t* sch) {
	autom_schedee_t* asch = mcast_out(sch, autom_schedee_t, sch);
	asch->func(asch->privdata);
}

const struct schedee_operations autom_schedee_operations = {
	.execute = &autom_schedee_execute,
	.displace = &do_nothing,
};

static inline void autom_schedee_init(autom_schedee_t* asch, autom_func_t func, void* privdata) {
	schedee_init(&asch->sch, &autom_schedee_operations);
	asch->func = func;
	asch->privdata = privdata;
}

#endif