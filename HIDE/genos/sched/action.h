#ifndef GENOS_ACTION_SCHEDEE_H
#define GENOS_ACTION_SCHEDEE_H

#include <genos/sched/schedee.h>
#include <gxx/event/delegate.h>

namespace genos {
	genos::schedee_ptr create_action_schedee(gxx::action dlg);
}

#endif