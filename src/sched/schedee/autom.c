#include <sched/sched.h>
#include <stdlib.h>

#include <gxx/panic.h>

struct autom_schedee {
	struct schedee sch;

	void* (*task) (void*, int*);
	void * arg;
	int state;
};

static void autom_schedee_execute(struct schedee* sch); 
static void autom_schedee_displace(struct schedee* sch);
static void autom_schedee_finalize(struct schedee* sch);

struct schedee_operations autom_schedee_op = {
	.execute = autom_schedee_execute,
	.displace = autom_schedee_displace,
	.finalize = autom_schedee_finalize,
};

struct schedee * create_autom_schedee(void* (*task) (void*, int*), void * arg) 
{
	struct autom_schedee * sch = (struct autom_schedee *) 
		malloc(sizeof(struct autom_schedee));

	schedee_init(&sch->sch, 0, &autom_schedee_op);

	sch->sch.flag.can_displace = 1;
	sch->task = task;
	sch->arg = arg;

	return &sch->sch;
}

static void autom_schedee_execute(struct schedee* sch) {
	struct autom_schedee * asch = mcast_out(sch, struct autom_schedee, sch);
	asch->task(asch->arg, &asch->state);
} 

static void autom_schedee_displace(struct schedee* sch) {
	sch->flag.runned = 0;
}

static void autom_schedee_finalize(struct schedee* sch) {
	struct autom_schedee * asch = mcast_out(sch, struct autom_schedee, sch);
	free(asch);	
}