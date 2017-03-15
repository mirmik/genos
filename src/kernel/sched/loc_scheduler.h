#ifndef LOCAL_SCHEDULER_H
#define LOCAL_SCHEDULER_H

#include <kernel/sched/scheduler.h>
#include <kernel/sched/stubschedee.h>
#include <kernel/tasks/tasklet.h>

extern struct dlist_head runlist[PRIORITY_TOTAL];
extern struct dlist_head waitlist;
extern struct dlist_head finallist;

extern void kernel_schedule_empty();

#endif