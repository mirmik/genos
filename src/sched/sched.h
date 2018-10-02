#ifndef GENOS_SCHED_SCHED_H
#define GENOS_SCHED_SCHED_H

#include <sched/schedee.h>

__BEGIN_DECLS

void schedee_run(struct schedee * sch);
void schedee_wait(struct schedee * sch);
void schedee_stop(struct schedee * sch);
void schedee_final(struct schedee * sch);

void schedee_manager_init();
void schedee_manager();

void __schedule__();
int  __displace__();

struct schedee * create_autom_schedee(void*(*task)(void*, int*), void* arg);
struct schedee * create_cooperative_schedee(void*(*task)(void*), void* arg, int heapsize);
//struct schedee * create_process(int(*task)(void*), void* arg, int heapsize);

__END_DECLS

#endif