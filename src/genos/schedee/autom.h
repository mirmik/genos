#ifndef GENOS_SCHED_AUTOM_SCHEDEE_H
#define GENOS_SCHED_AUTOM_SCHEDEE_H

#include <genos/sched.h>
#include <stdlib.h>

typedef void* (*autom_schedee_task_t) (void*, int*);

namespace genos
{
	class autom_schedee_base : public schedee
	{
	protected:
		int state = 0;

		autom_schedee_base() 
		{
			this -> state = 0;
			schedee_init(this, 0);
			this->flag.can_displace = 1;
		}

	public:
		int displace() override;
		void finalize() override {}
		void signal_handler(int sig) override {}	
	};

	class autom_schedee : public autom_schedee_base
	{
		autom_schedee_task_t task;
		void * arg;
	
	public:
		autom_schedee(){}

		autom_schedee(autom_schedee_task_t task, void* arg) : autom_schedee_base()
		{
			this -> task = task;
			this -> arg = arg;
		}

		void init(autom_schedee_task_t task, void* arg)
		{
			this -> task = task;
			this -> arg = arg;
		}

		void execute() override;
		void finalize() override;
	};
}

#define AUTOM_SCHEDEE(name, func, arg) \
struct autom_schedee name = { SCHEDEE_INIT(name), func, arg, 0 }

#endif