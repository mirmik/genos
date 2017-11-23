#ifndef GENOS_AUTOM_SCHEDEE_H
#define GENOS_AUTOM_SCHEDEE_H

#include <sys/cdefs.h>
#include "kernel/sched/schedee.h"
#include "kernel/sched/query.h"
#include "genos/sigslot/delegate.h"

struct autom_schedee : public schedee {
	query qry;
	action dlg;

	//schedee methods
	void execute();
	void systick_handler();
	void signal_handler();

	autom_schedee(action dlg, int prio);
};

#endif