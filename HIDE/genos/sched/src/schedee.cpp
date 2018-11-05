#include <genos/sched/schedee.h>

namespace genos {
	genos::schedee_manager_class schedee_manager;
}
/*
void genos::schedee::unwait(schedee& sch, uint8_t type) {
	dprln("unwait");
	genos::schedee_manager.unwait(*this, type);
}*/

void genos::schedee::unwait_wstate() {
	//dprln("genos::schedee::unwait_wstate");
	genos::schedee_manager.unwait(*this, SCHEDEE_STATE_WAIT);
}
