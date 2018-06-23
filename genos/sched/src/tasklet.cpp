#include <genos/sched/tasklet.h>
#include <gxx/syslock.h>

namespace genos  {
	genos::tasklet_manager_class tasklet_manager;
}

void genos::tasklet::plan() {
	genos::tasklet_manager.plan(*this);
}

void genos::tasklet_manager_class::plan(genos::tasklet& tsklt) {
	gxx::system_lock();
	planed_list.move_back(tsklt);
	gxx::system_unlock();
}

void genos::tasklet_manager_class::execute() {
	//atomic_section_enter();
	gxx::system_lock();
	while(!planed_list.empty()) {
		genos::tasklet& tsklt = *planed_list.begin();
		planed_list.pop(tsklt);
		gxx::system_unlock();
		tsklt.dlg();
		gxx::system_lock();
	}
	gxx::system_unlock();
}