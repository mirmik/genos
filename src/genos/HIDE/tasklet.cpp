#include <genos/tasklet.h>
#include <genos/atomic.h>

namespace genos  {
	genos::tasklet_manager_class tasklet_manager;
}

void genos::tasklet::plan() {
	genos::tasklet_manager.plan(*this);
}

void genos::tasklet_manager_class::plan(genos::tasklet& tsklt) {
	atomic_section_enter();
	planed_list.move_back(tsklt);
	atomic_section_leave();
}

void genos::tasklet_manager_class::execute() {
	atomic_section_enter();
	while(!planed_list.empty()) {
		genos::tasklet& tsklt = *planed_list.begin();
		planed_list.pop(tsklt);
		atomic_section_leave();
		tsklt.dlg();
		atomic_section_enter();
	}
	atomic_section_leave();
}