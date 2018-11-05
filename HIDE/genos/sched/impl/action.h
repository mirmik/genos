#ifndef GENOS_ACTION_IMPL_H
#define GENOS_ACTION_IMPL_H

#include <genos/sched/schedee.h>

namespace genos {
	class action_schedee : public schedee {
		gxx::action dlg;

	public:
		action_schedee(gxx::action act);
		void execute() override;
		void finalize() override;		
		void displace() override;
	};
}

#endif