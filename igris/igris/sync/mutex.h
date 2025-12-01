#ifndef IGRIS_SYNC_MUTEX_H
#define IGRIS_SYNC_MUTEX_H

#include <igris/compiler.h>
#include <igris/datastruct/dlist.h>

struct igris_mutex
{
    struct dlist_head wait_list;
    int locked;
};

typedef struct igris_mutex igris_mutex_t;

__BEGIN_DECLS

void igris_mutex_init(struct igris_mutex *mtx);
int igris_mutex_lock(struct igris_mutex *mtx);
int igris_mutex_trylock(struct igris_mutex *mtx);
int igris_mutex_unlock(struct igris_mutex *mtx);
int igris_mutex_destroy(struct igris_mutex *mtx);

__END_DECLS

#ifdef __cplusplus
namespace igris
{
    class mutex
    {
        igris_mutex_t m_mutex = {};

    public:
        mutex() { igris_mutex_init(&m_mutex); }

        mutex(const mutex &) = delete;
        mutex &operator=(const mutex &) = delete;

        ~mutex() { igris_mutex_destroy(&m_mutex); }

        void lock() { igris_mutex_lock(&m_mutex); }

        bool try_lock() { return igris_mutex_trylock(&m_mutex) == 0; }

        void unlock() { igris_mutex_unlock(&m_mutex); }

        igris_mutex_t *native_handle() { return &m_mutex; }
    };

    class mutex_guard
    {
        mutex &m_mutex;

    public:
        explicit mutex_guard(mutex &mtx) : m_mutex(mtx)
        {
            m_mutex.lock();
        }

        ~mutex_guard()
        {
            m_mutex.unlock();
        }
    };
}
#endif

#ifdef IGRIS_SYNC_ENABLE_MUTEX_COMPAT_NAMES
#define mutex_init igris_mutex_init
#define mutex_lock igris_mutex_lock
#define mutex_trylock igris_mutex_trylock
#define mutex_unlock igris_mutex_unlock
#define mutex_destroy igris_mutex_destroy
#endif

#endif
