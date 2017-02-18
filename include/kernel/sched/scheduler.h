#ifndef GENOS_SCHEDULER_H
#define GENOS_SCHEDULER_H

#include <kernel/sched/schedee.h>

#ifndef PRIORITY_TOTAL
#	define PRIORITY_TOTAL 6
#endif

class schedee;

void set_state_run(schedee *sch);
void set_state_wait(schedee *sch);
void set_state_final(schedee *sch);
void set_state_zombie(schedee *sch);
void set_final_deallocate(schedee *sch, bool en);
void set_final_release(schedee *sch, bool en);

bool state_is_run(schedee *sch);
bool state_is_wait(schedee *sch);
bool state_is_zombie(schedee *sch);
bool is_final_deallocated(schedee *sch);
bool is_final_release(schedee *sch);

void genos_idle();
void __schedule__() __attribute__((noreturn));

void scheduler_free(void*);

#endif