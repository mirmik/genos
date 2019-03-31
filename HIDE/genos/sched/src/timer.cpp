#include <genos/sched/timer.h>
#include <igris/sync/syslock.h>

#include <gxx/print.h>

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
	
	time_t final = tim.timer.final();

	for(; it != end; it++) {
		if (final - (*it).timer.final() < 0) break;
	}
	
	planed_list.move_prev(tim, it);
}

void genos::timer_manager_class::unbind(genos::timer_tasklet& tim) {
	planed_list.pop(tim);
}

void genos::timer_manager_class::execute() {
	system_lock();
	time_t now = jiffies();
	system_unlock();

	//dprln("size: ", planed_list.size());
	
	while(!planed_list.empty()) {
		genos::timer_tasklet& timer = *planed_list.begin();
		//dprln(now, timer.timer.interval, timer.timer.start);
		if (timer.timer.check(now)) {
			planed_list.pop(timer);

			//dprln("dlg", now, timer.timer.interval);
			//gxx::print_dump(&timer, sizeof(timer));

			timer.dlg();
			//dprln("dlg");
			if (timer.m_autorepeat) {
				timer.timer.swift();
				plan(timer);
			}
		} 
		else break;
	}
}