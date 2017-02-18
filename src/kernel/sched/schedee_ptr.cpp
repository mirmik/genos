/*#include <kernel/sched/schedee_ptr.h>

#include <kernel/sched/schedee.h>
#include <kernel/sched/scheduler.h>


schedee_ptr::schedee_ptr(schedee* ptr) {
	sch = ptr;
	if (sch) {
		sch->ref++;
	}
}

schedee_ptr::schedee_ptr(const schedee_ptr& rhs) {
	sch = rhs.sch;
	if (sch) {
		sch->ref++;
	}
}

schedee_ptr::~schedee_ptr() {
	if (sch) {
		sch->release();
	}
}

void schedee_ptr::addref() {
	if(sch)
		sch->ref++;
}

schedee* schedee_ptr::operator->(){
	assert(sch);
	return sch;
}

schedee& schedee_ptr::operator*(){
	assert(sch);
	return *sch;
}*/