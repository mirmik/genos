#ifndef GENOS_PROCESS_MANAGER_H
#define GENOS_PROCESS_MANAGER_H

#include "kernel/sched/autom_schedee.h"
#include "genos/sigslot/delegate.h"

namespace process {

	id_t create_autom(delegate<void>, int prio);
	schedee* getsch(id_t id);

};

#endif