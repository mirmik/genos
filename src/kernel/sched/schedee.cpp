#include <kernel/sched/schedee.h>
#include <kernel/sched/scheduler.h>

schedee::schedee() : lnk() {
//	set_final_release(this,true);
}

schedee& schedee::Run() {
	schedee_set_state_run(this);
	return *this;
}

schedee& schedee::Stop() {
	schedee_set_state_wait(this, SCHEDEE_BLOCKED_STOP);
	return *this;
}

bool schedee::Completed() {
	return state_is_zombie(this);
}

void schedee::Exit() {
	schedee_set_state_final(this);
}

void schedee::final() {
	schedee_set_state_zombie(this);

	if (final_callback) final_callback(this);

	if (is_final_deallocated(this)) { 
		gxx::destructor(this);
		scheduler_free(this);
	}
}

schedee& schedee::final_deallocate(bool en) {
	set_final_deallocate(this, en);
	return *this;
}

schedee& schedee::final_handler(void(*f)(schedee*)) {
	//if (final_callback == nullptr);
	final_callback = f;
}