#ifndef LOCAL_SCHEDULER_H
#define LOCAL_SCHEDULER_H

#include <kernel/sched/scheduler.h>
#include <kernel/sched/stubschedee.h>
#include <kernel/tasks/tasklet.h>

#include <gxx/dlist.h>

namespace genos {

	using schlist_t = gxx::dlist<genos::schedee, &genos::schedee::schlnk>;

	extern schlist_t runlist[PRIORITY_TOTAL];
	extern schlist_t waitlist;
	extern schlist_t finallist;

	extern void kernel_schedule_empty();

}

#endif