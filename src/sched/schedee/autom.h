#ifndef GENOS_SCHED_AUTOM_SCHEDEE_H
#define GENOS_SCHED_AUTOM_SCHEDEE_H

#include <sched/sched.h>
#include <stdlib.h>

typedef void* (*autom_schedee_task_t) (void*, int*);

struct autom_schedee
{
	struct schedee sch;

	void* (*task) (void*, int*);
	void * arg;
	int state;
};


__BEGIN_DECLS

extern void autom_schedee_execute(struct schedee* sch);
extern void autom_schedee_displace(struct schedee* sch);
extern void autom_schedee_finalize(struct schedee* sch);

__END_DECLS

const struct schedee_operations autom_schedee_op =
{
	.execute = autom_schedee_execute,
	.displace = autom_schedee_displace,
	.finalize = autom_schedee_finalize,
};


__BEGIN_DECLS
static inline 
void autom_schedee_init(struct autom_schedee* asch,
                                 autom_schedee_task_t task,
                                 void* arg)
{
	asch -> task = task;
	asch -> arg = arg;
	asch -> state = 0;
	schedee_init(&asch->sch, 0, &autom_schedee_op);
	
	asch->sch.flag.can_displace = 1;
}
__END_DECLS


#define AUTOM_SCHEDEE(name, func, arg) \
struct autom_schedee name = { SCHEDEE_INIT(name), func, arg, 0 }

#endif