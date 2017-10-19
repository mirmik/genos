#ifndef GENOS_SCHED_SLEEP_H
#define GENOS_SCHED_SLEEP_H

#include <sys/cdefs.h>
#include <genos/sync/atomic.h>
#include <genos/sched/scheduler.h>

__BEGIN_DECLS

static void sleep_for_milliseconds(int ms) {
	atomic_section_enter();

	atomic_section_leave();
	__displace__();
}

__END_DECLS

#endif