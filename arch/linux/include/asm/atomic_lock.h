#ifndef GENOS_LINUX_ATOMIC_LOCK_H
#define GENOS_LINUX_ATOMIC_LOCK_H

#include <mutex>

class lock_atomic {
	static std::mutex atomic_mutex;

public:
	lock_atomic() {
		atomic_mutex.lock();
	}

	~lock_atomic() {
		atomic_mutex.unlock();
	}
};

#endif