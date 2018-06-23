#ifndef GENOS_TIMER_TASKLET_H
#define GENOS_TIMER_TASKLET_H

#include <gxx/util/setget.h>
#include <gxx/container/dlist.h>
#include <gxx/event/delegate.h>
#include <genos/time/systime.h>
#include <genos/time/timer.h>

namespace genos {
	class timer_tasklet {
	public:
		dlist_head lnk;
		gxx::action dlg;
		genos::timer<systime::time_t> timer;

		bool m_autorepeat;

		timer_tasklet(gxx::action dlg, systime::time_t interval, systime::time_t start) : dlg(dlg), timer{start,interval} {}
		timer_tasklet(gxx::action dlg, systime::time_t interval) : dlg(dlg), timer{systime::millis(),interval} {}

		void plan();
		void unbind();
		gxx::action make_plan_delegate() { return gxx::action(&timer_tasklet::plan, this); }

		FLOW_ACCESSOR(autorepeat, m_autorepeat);
	};

	class timer_manager_class {
		gxx::dlist<timer_tasklet, &timer_tasklet::lnk> planed_list;

	public:
		void plan(timer_tasklet& tsklt);
		void unbind(timer_tasklet& tsklt);
		void execute();
	};

	extern timer_manager_class timer_manager;
}

#endif