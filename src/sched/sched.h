#ifndef GENOS_SCHED_SCHED_H
#define GENOS_SCHED_SCHED_H

#include <sched/schedee.h>

#define DISPLACE_REAL 0
#define DISPLACE_VIRTUAL 1
#define DISPLACE_ERROR -1

__BEGIN_DECLS

void schedee_run(struct schedee * sch);
void schedee_wait(struct schedee * sch);
void __schedee_wait_for(struct schedee * parent, struct schedee * child);
void schedee_stop(struct schedee * sch);
void schedee_final(struct schedee * sch);

void schedee_manager_init();
void scheduler_init();

void schedee_manager_step();

void __schedule__();
int  __displace__();

struct schedee * create_autom_schedee(void*(*task)(void*, int*), void* arg);
struct schedee * create_cooperative_schedee(void*(*task)(void*), void* arg, int heapsize);
//struct schedee * create_process(int(*task)(void*), void* arg, int heapsize);

int schedee_manager_total_planed();

void schedee_manager_debug_info(); 
	
__END_DECLS

#endif