#ifndef GENOS_AUTOM_SCHEDEE_H
#define GENOS_AUTOM_SCHEDEE_H

#include <genos/sigslot/delegate.h>

namespace Kernel {

	class automSchedee : public schedee {
	public:
		delegate<void> automate;

		void execute() {
			automate();
		};

		automSchedee(delegate<void> _dlg, int prio) : schedee() {
			automate = _dlg;
			sta_prio = prio;
			dyn_prio = prio;
		}

	};

}

#endif