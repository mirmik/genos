#ifndef GENOS_SCHED_SCHED_H
#define GENOS_SCHED_SCHED_H

#include <sched/schedee.h>

__BEGIN_DECLS

void schedee_run(struct schedee * sch);
void schedee_wait(struct schedee * sch);
void schedee_stop(struct schedee * sch);
void schedee_finish(struct schedee * sch);

void schedee_manager();

int __displace__();
int displace_automic();
int displace_substitute();

struct schedee * current_schedee();

struct schedee * create_autom_schedee(int(*task)(void*), void* arg);
struct schedee * create_process(int(*task)(void*), void* arg, int heapsize);

__END_DECLS

#endif