#ifndef IGRIS_SEM_H
#define IGRIS_SEM_H

#include <igris/datastruct/dlist.h>
#include <igris/sync/syslock.h>

struct igris_semaphore
{
    struct dlist_head wait_list;
    int count;
};

typedef struct igris_semaphore igris_sem_t;

#define IGRIS_SEMAPHORE_INIT(name, n)                                          \
    {                                                                          \
        .wait_list = DLIST_HEAD_INIT((name).wait_list), .count = n,            \
    }

#define IGRIS_SEMAPHORE(name, n)                                               \
    struct igris_semaphore name = IGRIS_SEMAPHORE_INIT(name, n)

__BEGIN_DECLS

void igris_sem_init(struct igris_semaphore *sem, int shared, int val);

int igris_sem_wait(struct igris_semaphore *sem);

int igris_sem_trywait(struct igris_semaphore *sem);

int igris_sem_post(struct igris_semaphore *sem);

int igris_sem_getvalue(struct igris_semaphore *sem, int *val);

int igris_sem_destroy(struct igris_semaphore *sem);

__END_DECLS

#ifdef __cplusplus
namespace igris
{
    class semaphore
    {
        igris_sem_t sem = {};

    public:
        semaphore(int val = 1)
        {
            igris_sem_init(&sem, 0, val);
        }
        semaphore(const semaphore &) = delete;
        semaphore(semaphore &&) = delete;

        ~semaphore()
        {
            igris_sem_destroy(&sem);
        }

        void post()
        {
            igris_sem_post(&sem);
        }
        void wait()
        {
            igris_sem_wait(&sem);
        }
        int trywait()
        {
            return igris_sem_trywait(&sem);
        }
        int getvalue()
        {
            int val;
            igris_sem_getvalue(&sem, &val);
            return val;
        }
    };
}
#endif

#if defined(IGRIS_SYNC_ENABLE_SEM_COMPAT_NAMES)
typedef igris_sem_t sem_t;
#define SEMAPHORE_INIT(name, n) IGRIS_SEMAPHORE_INIT(name, n)
#define SEMAPHORE(name, n) IGRIS_SEMAPHORE(name, n)
#define sem_init igris_sem_init
#define sem_wait igris_sem_wait
#define sem_trywait igris_sem_trywait
#define sem_post igris_sem_post
#define sem_getvalue igris_sem_getvalue
#define sem_destroy igris_sem_destroy
#endif

#endif
