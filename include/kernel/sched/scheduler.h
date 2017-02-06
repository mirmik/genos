#ifndef GENOS_SCHEDULER_H
#define GENOS_SCHEDULER_H

#include <kernel/sched/schedee.h>

#ifndef PRIORITY_TOTAL
#	define PRIORITY_TOTAL 6
#endif

void set_state_run(schedee *sch);
void set_state_wait(schedee *sch);
void set_state_zombie(schedee *sch);

#endif