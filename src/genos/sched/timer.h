#ifndef GENOS_TIMER_H
#define GENOS_TIMER_H

#include <gxx/util/setget.h>
#include <gxx/dlist.h>
#include <gxx/event/delegate.h>
#include <genos/sysclock.h>

namespace genos {
	class timer {
	public:
		dlist_head lnk;
		gxx::action dlg;

		genos::sysclock::time_t start;
		genos::sysclock::time_t interval;
		bool m_autorepeat;

		bool check() { return check(systime::millis()); };
		bool check(uint32_t now) { return (int32_t)now - (int32_t)(start + interval) >= 0; }

		uint32_t final() { return start + interval; }
		void swift() { start += interval; }

		timer(gxx::action dlg, genos::sysclock::time_t interval, genos::sysclock::time_t start) : dlg(dlg), start(start), interval(interval) {}
		timer(gxx::action dlg, genos::sysclock::time_t interval) : dlg(dlg), start(systime::millis()), interval(interval) {}

		void plan();
		void unbind();
		gxx::action make_plan_delegate() { return gxx::action(&timer::plan, this); }

		FLOW_ACCESSOR(autorepeat, m_autorepeat);
	};

	class timer_manager_class {
		gxx::dlist<timer, &timer::lnk> planed_list;

	public:
		void plan(timer& tsklt);
		void unbind(timer& tsklt);
		void execute();
	};

	extern timer_manager_class timer_manager;
}

#endif