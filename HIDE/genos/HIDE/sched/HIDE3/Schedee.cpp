#include <kernel/sched/Schedee.h>
#include <kernel/sched/SchedeeTable.h>

int8_t Genos::run(pid_t pid) {
	Genos::Schedee* sch = Genos::Glue::getSchedee(pid);
	if (!sch) return -1;
	sch->run();
	return 0;
}

int8_t Genos::stop(pid_t pid) {
	Genos::Schedee* sch = Genos::Glue::getSchedee(pid);
	if (!sch) return -1;
	sch->stop();
	return 0;
}

int8_t Genos::final(pid_t pid) {
	Genos::Schedee* sch = Genos::Glue::getSchedee(pid);
	if (!sch) return -1;
	sch->final();
	return 0;
}

Genos::Schedee* Genos::raw(pid_t pid) {
	return Genos::Glue::getSchedee(pid);
}

int8_t Genos::assign_name(pid_t pid, const char* name) {
	Genos::Schedee* sch = Genos::Glue::getSchedee(pid);
	if (!sch) return -1;
	sch->name(name);
	return 0;
}