#include <kernel/csection.h>

#include <mutex>

static std::mutex lock;

void atomic_section_enter() {
	lock.lock();
}

void atomic_section_leave() {
	lock.unlock();
}