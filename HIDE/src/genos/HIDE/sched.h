#ifndef GENOS_SCHED_SCHED_H
#define GENOS_SCHED_SCHED_H

#include <genos/schedee.h>

extern igris::console_command schedee_manager_utilities[];

__BEGIN_DECLS

void schedee_run(struct schedee * sch);
void schedee_wait(struct schedee * sch);
void schedee_stop(struct schedee * sch);
void __schedee_final(struct schedee * sch);

void schedee_exit();

void schedee_manager_init();
void scheduler_init();

void schedee_manager_step();

void __context_displace_vector__() __attribute__((noreturn));
void __schedule__();
int  __displace__();

struct schedee * create_autom_schedee(void*(*task)(void*, int*), void* arg);
struct schedee * create_cooperative_schedee(void*(*task)(void*), void* arg, int heapsize);
struct schedee * create_process(int(*task)(void*), void* arg, int heapsize);

int schedee_manager_total_planed();
void schedee_manager_debug_info(); 
	
__END_DECLS

#endif