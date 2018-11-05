#include <genos/sched/action.h>
#include <genos/sched/impl/action.h>

genos::schedee_ptr genos::create_action_schedee(gxx::action dlg) {
	genos::schedee* sch = new genos::action_schedee(dlg);
	schedee_manager.run(*sch);
	return sch;
}

genos::action_schedee::action_schedee(gxx::action act) : dlg(act) {}

void genos::action_schedee::execute() {
	dlg();
	schedee_manager.final(*this);
}

void genos::action_schedee::finalize() {
	dprln("finalize action");
	delete this;
}

void genos::action_schedee::displace() {}
