#include <sched/Schedee.h>

Genos::Schedee* __current_schedee; 

void Genos::currentSchedee(Genos::Schedee* sch) {
	__current_schedee = sch;
}

Genos::Schedee* Genos::currentSchedee() {
	return __current_schedee;
}

void genos::current_schedee(Genos::Schedee* sch) {
	__current_schedee = sch;
}

Genos::Schedee* genos::current_schedee() {
	return __current_schedee;
}