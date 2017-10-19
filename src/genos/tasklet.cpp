#include <genos/tasklet.h>

namespace genos  {
	genos::tasklet_manager_class tasklet_manager;
}

void genos::tasklet::plan() {
	genos::tasklet_manager.plan(*this);
}

void genos::tasklet_manager_class::plan(genos::tasklet& tsklt) {
	planed_list.move_back(tsklt);
}

void genos::tasklet_manager_class::execute() {
	while(!planed_list.empty()) {
		genos::tasklet& tsklt = *planed_list.begin();
		planed_list.pop(tsklt);
		tsklt.dlg();
	}
}