#include <genos/proclib.h>
#include <genos/sched/schedee.h>
#include <genos/sched/timer.h>
#include <genos/schedule.h>
#include <gxx/print.h>

void genos::sleep(int ms) {
	genos::schedee* cur = genos::schedee_manager.current_schedee;
	//dprln("sleep ms: ", systime::millis());
	genos::timer_tasklet tim(gxx::make_delegate(&genos::schedee::unwait_wstate, cur), systime::ms2j(ms));
	genos::schedee_manager.wait(*cur, genos::SCHEDEE_STATE_WAIT);

	
	tim.plan();
	genos::displace();	

	//dprln("return_frow_sleep");			
}

void genos::set_wait_handler(gxx::delegate<void>& dlg) {
	genos::schedee* cur = genos::schedee_manager.current_schedee;
	dlg = gxx::make_delegate(&genos::schedee::unwait_wstate, cur);
}

void genos::wait() {
	genos::schedee* cur = genos::schedee_manager.current_schedee;
	genos::schedee_manager.wait(*cur, genos::SCHEDEE_STATE_WAIT);
	genos::displace();	
}
