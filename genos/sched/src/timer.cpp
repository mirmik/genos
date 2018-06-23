#include <genos/sched/timer.h>
#include <gxx/syslock.h>

namespace genos {
	timer_manager_class timer_manager;
}

void genos::timer_tasklet::plan() {
	genos::timer_manager.plan(*this);
}

void genos::timer_tasklet::unbind() {
	genos::timer_manager.unbind(*this);
}

void genos::timer_manager_class::plan(genos::timer_tasklet& tim) {
	auto it = planed_list.begin();
	auto end = planed_list.end();
	
	uint32_t final = tim.timer.final();

	for(; it != end; it++) {
		if ((int32_t)final - (int32_t)(*it).timer.final() < 0) break;
	}
	
	planed_list.move_prev(tim, it);
}

void genos::timer_manager_class::unbind(genos::timer_tasklet& tim) {
	planed_list.pop(tim);
}

void genos::timer_manager_class::execute() {
	gxx::system_lock();
	auto now = systime::now();
	gxx::system_unlock();
	
	while(!planed_list.empty()) {
		genos::timer_tasklet& timer = *planed_list.begin();
		if (timer.timer.check(now)) {
			planed_list.pop(timer);
			timer.dlg();
			if (timer.m_autorepeat) {
				timer.timer.swift();
				plan(timer);
			}
		} 
		else break;
	}
}