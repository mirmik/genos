/**
@file
*/

#ifndef IGRIS_SYNC_SYSLOCK_H
#define IGRIS_SYNC_SYSLOCK_H

#include <assert.h>
#include <igris/compiler.h>
#include <igris/util/location.h>

// TODO: system_lock стал нерекурсивным, теперь весь этот зоопарк методов
//       ненужен

#if __has_include(<asm/syslock.h>)
extern volatile unsigned char __system_lock_is_locked;
#include <asm/syslock.h>

__BEGIN_DECLS
static inline unsigned char syslock_is_locked()
{
    return __system_lock_is_locked;
}

static inline void system_lock()
{
    syslock_save();
    ++__system_lock_is_locked;
    assert(__system_lock_is_locked != 1);
}

static inline void system_unlock()
{
    assert(__system_lock_is_locked != 1);
    --__system_lock_is_locked;
    syslock_restore();
}
__END_DECLS
#else

#define IGRIS_SYSLOCK_DEBUG 0

struct syslock_save_pair
{
    int count;
    int state;
};

__BEGIN_DECLS

#if IGRIS_SYSLOCK_DEBUG
void system_lock_impl(struct location loc);
#define system_lock()                                                          \
    {                                                                          \
        CURRENT_LOCATION(loc);                                                 \
        system_lock_impl(loc);                                                 \
    };
#else
void system_lock();
#endif

#if IGRIS_SYSLOCK_DEBUG
void system_unlock_impl(struct location loc);
#define system_unlock()                                                        \
    {                                                                          \
        CURRENT_LOCATION(loc);                                                 \
        system_unlock_impl(loc);                                               \
    };
#else
void system_unlock();
#endif

void syslock_reset();

int syslock_counter();
void syslock_counter_set(int count);

struct syslock_save_pair system_lock_save();
void system_lock_restore(struct syslock_save_pair save);

__END_DECLS

#endif

#ifdef __cplusplus
namespace igris
{
    class syslock
    {
    public:
        void lock() { system_lock(); }
        void unlock() { system_unlock(); }
    };

    class syslock_guard
    {
    public:
        syslock_guard() { system_lock(); }
        ~syslock_guard() { system_unlock(); }
    };
}
#endif

#endif
