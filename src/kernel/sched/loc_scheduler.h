#ifndef LOCAL_SCHEDULER_H
#define LOCAL_SCHEDULER_H

#include <kernel/sched/scheduler.h>
//#include <kernel/sched/stubschedee.h>
//#include <kernel/tasks/tasklet.h>

#include <gxx/dlist.h>

namespace genos {

using schlst_t = gxx::dlist<schedee, &schedee::schlnk>;

extern schlst_t runlist[PRIORITY_TOTAL];
extern schlst_t waitlist;
extern schlst_t finallist;

extern void kernel_schedule_empty();

}

#endif