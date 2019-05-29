#ifndef GENOS_SCHED_AUTOM_SCHEDEE_H
#define GENOS_SCHED_AUTOM_SCHEDEE_H

#include <sched/sched.h>
#include <stdlib.h>

typedef void* (*autom_schedee_task_t) (void*, int*);

extern const struct schedee_operations autom_schedee_op;

struct autom_schedee : public schedee
{
	void* (*task) (void*, int*);
	void * arg;
	int state;

	void init(autom_schedee_task_t task, void* arg)
	{
		this -> task = task;
		this -> arg = arg;
		this -> state = 0;
		schedee_init(this, 0, &autom_schedee_op);

		this->flag.can_displace = 1;
	}
};

__BEGIN_DECLS

__attribute__((deprecated))
static inline
void autom_schedee_init(struct autom_schedee* asch,
                        autom_schedee_task_t task,
                        void* arg)
{
	asch -> task = task;
	asch -> arg = arg;
	asch -> state = 0;
	schedee_init(asch, 0, &autom_schedee_op);

	asch->flag.can_displace = 1;
}
__END_DECLS


#define AUTOM_SCHEDEE(name, func, arg) \
struct autom_schedee name = { SCHEDEE_INIT(name), func, arg, 0 }

#endif