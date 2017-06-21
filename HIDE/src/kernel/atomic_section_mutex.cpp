#include <kernel/csection.h>

#include <debug/dprint.h>
#include <mutex>

static std::recursive_mutex lock;

void atomic_section_enter() {
	//dprln("atomic_section_enter");
	lock.lock();
}

void atomic_section_leave() {
	//dprln("atomic_section_leave");
	lock.unlock();
}