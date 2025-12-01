#include <cassert>
#include <igris/sync/syslock.h>
#include <mutex>

#if __has_include(<unistd.h>)
#include <unistd.h>
int __is_locked = -1;
#endif

#include <igris/dprint.h>

static std::recursive_mutex mtx;
// static std::mutex mtx;
static thread_local int count = 0;

__BEGIN_DECLS

#if IGRIS_SYSLOCK_DEBUG
void system_lock_impl(struct location loc)
{
#else
void system_lock()
{
#endif
    mtx.lock();

#if IGRIS_SYSLOCK_DEBUG
    if (__is_locked == getpid())
    {
        dprln("recurse system lock is deprecated");
        debug_print_location(loc);
    }
#endif

#if __has_include(<unistd.h>)
    __is_locked = getpid();
#endif

    if (count == 0)
    {
        // save = tmpsave;
    }

    ++count;
    assert(count < 10);
}

#if IGRIS_SYSLOCK_DEBUG
void system_unlock_impl(struct location loc)
{
    if (__is_locked != getpid())
    {
        dprln("trying unlock unlocked lock (or use recursive unlock)");
        debug_print_location(loc);
        // abort();
    }
#else
void system_unlock()
{
#endif
    --count;
    assert(count >= 0);

#if __has_include(<unistd.h>)
    __is_locked = -1;
#endif

    mtx.unlock();
}

struct syslock_save_pair system_lock_save()
{
    auto ret = syslock_save_pair{count, 0};
    assert(count != 0);

    while (count--)
    {
        mtx.unlock();
    }

    return ret;
}

void system_lock_restore(struct syslock_save_pair save)
{
    mtx.lock();

    count = save.count;
    int curcount = 1;

    while (curcount != count)
    {
        curcount++;
        mtx.lock();
    }
}

void syslock_reset() { count = 0; }

int syslock_counter() { return count; }

void syslock_counter_set(int newcount) { count = newcount; }

__END_DECLS
