#include <kernel/sched/Schedee.h>
#include <kernel/sched/SchedeeTable.h>

int8_t Genos::run(pid_t pid) {
	Genos::Schedee* sch = Genos::Glue::getSchedee(pid);
	sch->run();
}

int8_t Genos::stop(pid_t pid) {
	Genos::Schedee* sch = Genos::Glue::getSchedee(pid);
	sch->stop();
}

int8_t Genos::final(pid_t pid) {
	Genos::Schedee* sch = Genos::Glue::getSchedee(pid);
	sch->final();
}

Genos::Schedee* Genos::raw(pid_t pid) {
	return Genos::Glue::getSchedee(pid);
}