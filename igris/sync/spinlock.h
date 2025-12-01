#ifndef IGRIS_SYNC_SPINLOCK_H
#define IGRIS_SYNC_SPINLOCK_H

#include <igris/displace.h>

struct spinlock
{
    int8_t locked;
};

__BEGIN_DECLS

void spinlock_init(struct spinlock *lock) { lock->locked = 0; }

void spin_lock(struct spinlock *lock)
{
    system_lock();
    while (lock->locked)
    {
        system_unlock();
        __displace__();
        system_lock();
    }
    lock->locked = 1;
    system_unlock();
}

void spin_unlock(struct spinlock *lock) { lock->locked = 0; }

__END_DECLS

#endif
