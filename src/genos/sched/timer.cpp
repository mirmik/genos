#include <genos/timer.h>

namespace genos {
	timer_manager_class timer_manager;
}

void genos::timer::plan() {
	genos::timer_manager.plan(*this);
}

void genos::timer::unbind() {
	genos::timer_manager.unbind(*this);
}

void genos::timer_manager_class::plan(genos::timer& tim) {
	auto it = planed_list.begin();
	auto end = planed_list.end();
	
	uint32_t final = tim.final();

	for(; it != end; it++) {
		if ((int32_t)final - (int32_t)(*it).final() < 0) break;
	}
	
	planed_list.move_prev(tim, it);
}

void genos::timer_manager_class::unbind(genos::timer& tim) {
	planed_list.pop(tim);
}

void genos::timer_manager_class::execute() {
	auto now = sysclock.jiffies();
	
	while(!planed_list.empty()) {
		genos::timer& timer = *planed_list.begin();
		if (timer.check(now)) {
			planed_list.pop(timer);
			timer.dlg();
			if (timer.m_autorepeat) {
				timer.swift();
				plan(timer);
			}
		} 
		else break;
	}
}