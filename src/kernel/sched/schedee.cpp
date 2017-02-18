#include <kernel/sched/schedee.h>
#include <kernel/sched/scheduler.h>

schedee::schedee(schedee* parent) : lnk(), parent( parent ) {
	set_final_release(this,true);
}

schedee& schedee::Run() {
	set_state_run(this);
	return *this;
}

schedee& schedee::Stop() {
	set_state_wait(this);
	return *this;
}

bool schedee::Completed() {
	return state_is_zombie(this);
}

void schedee::Exit() {
	set_state_final(this);
}

void schedee::final() {
	set_state_zombie(this);

	if (final_callback) final_callback(this, result);

	if (is_final_deallocated(this)) { 
		gxx::destructor(this);
		scheduler_free(this);
	}
	//if (is_final_release(this)) {
	//	release();
	//}
}

/*void schedee::release() {
	ref--;
	if (ref == 0) {
		if (is_final_deallocated(this)) { 
			gxx::destructor(this);
			scheduler_free(this);
		}
	}
}*/

schedee& schedee::final_deallocate(bool en) {
	set_final_deallocate(this, en);
	return *this;
}
/*
schedee& schedee::final_release(bool en) {
	set_final_release(this, en);
	return *this;
}*/

//schedee& schedee::result_deallocate(bool en) {
//	set_result_deallocate(this, en);
//	return *this;
//}

schedee& schedee::final_handler(void(*f)(schedee*, void*)) {
	//if (final_callback == nullptr);
	final_callback = f;
}

schedee& schedee::final_handler(void(*f)()) {
	return final_handler((void(*)(schedee*, void*)) f);
}

schedee& schedee::Result(void* res) {
	result = res;
	return *this;
}

void* schedee::Result() {
	return result;
}