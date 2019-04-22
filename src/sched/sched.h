#ifndef GENOS_SCHED_SCHED_H
#define GENOS_SCHED_SCHED_H

#include <sched/schedee.h>

__BEGIN_DECLS

void schedee_run(struct schedee * sch);
void schedee_wait(struct schedee * sch);
void __schedee_wait_for(struct schedee * parent, struct schedee * child);
void schedee_stop(struct schedee * sch);
void schedee_final(struct schedee * sch);

void schedee_manager_init();
//void schedee_manager_step();

void __schedule__();
int  __displace__();

struct schedee * create_autom_schedee(void*(*task)(void*, int*), void* arg);
struct schedee * create_cooperative_schedee(void*(*task)(void*), void* arg, int heapsize);
//struct schedee * create_process(int(*task)(void*), void* arg, int heapsize);

__END_DECLS

#ifdef __cplusplus
#include <igris/event/delegate.h>

namespace genos {
	static inline
	struct schedee * create_autom_schedee(void*(*task)(void*, int*), void* arg) 
	{
		return ::create_autom_schedee(task, arg);
	}

	static inline
	struct schedee * create_autom_schedee(igris::fastdelegate<void*,int*> fdlg) 
	{
		return ::create_autom_schedee(fdlg.extfunction, fdlg.object);
	}

	namespace schedee_manager 
	{
		void step();
		int total_planed();
	}
}

#endif

#endif