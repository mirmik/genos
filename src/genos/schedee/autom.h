#ifndef GENOS_SCHED_AUTOM_SCHEDEE_H
#define GENOS_SCHED_AUTOM_SCHEDEE_H

#include <genos/sched.h>
#include <stdlib.h>

typedef void* (*autom_schedee_task_t) (void*, int*);

class autom_schedee : public schedee
{
	void* (*task) (void*, int*);
	void * arg;
	int state;

public:
	void init(autom_schedee_task_t task, void* arg)
	{
		this -> task = task;
		this -> arg = arg;
		this -> state = 0;
		schedee_init(this, 0);

		this->flag.can_displace = 1;
	}

	void execute() override;
	int displace() override;
	void finalize() override; 
};

#define AUTOM_SCHEDEE(name, func, arg) \
struct autom_schedee name = { SCHEDEE_INIT(name), func, arg, 0 }

#endif