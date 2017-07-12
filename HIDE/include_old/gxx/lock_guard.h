#ifndef GXX_LOCK_GUARD_H
#define GXX_LOCK_GUARD_H

namespace gxx {
	template<typename Lock>
	class lock_guard {
		Lock &lock;

	public:
		lock_guard(Lock &reflock) : lock(reflock) {
			lock.lock();
		}

		~lock_guard() {
			lock.unlock();
		}
	};
}

#endif