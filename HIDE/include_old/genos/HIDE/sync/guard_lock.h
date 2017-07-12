#ifndef GENOS_SYNC_GUARD_LOCK_H
#define GENOS_SYNC_GUARD_LOCK_H

namespace gxx {

template<typename Lock>
class guard_lock
{
	L* _l;
	guard_lock(Lock& l) : _l(&l) { l.lock(); }
	~guard_lock() { l.unlock(); }
};

};

#endif