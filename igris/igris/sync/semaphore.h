#ifndef IGRIS_SEM_H
#define IGRIS_SEM_H

#if __has_include(<asm_igris/semaphore.h>)
#include <asm_igris/semaphore.h>
#elif __has_include(<semaphore.h>)
#include <semaphore.h>
#elif __has_include(<sys/semaphore.h>)
#include <sys/semaphore.h>
#else

#include <igris/datastruct/dlist.h>
#include <igris/sync/syslock.h>

typedef struct semaphore
{
    struct dlist_head wait_list;
    int count;
} sem_t;

#define SEMAPHORE_INIT(name, n)                                                \
    {                                                                          \
        .wait_list = DLIST_HEAD_INIT((name).wait_list), .count = n,            \
    }

#define SEMAPHORE(name, n) struct semaphore name = SEMAPHORE_INIT(name, n)

__BEGIN_DECLS

void sem_init(struct semaphore *sem, int shared, int val);

int sem_wait(struct semaphore *sem);

int sem_trywait(struct semaphore *sem);

int sem_post(struct semaphore *sem);

int sem_getvalue(struct semaphore *sem, int *val);

int sem_destroy(struct semaphore *sem);

__END_DECLS

#endif



#ifdef __cplusplus
namespace igris 
{
    class semaphore 
    {
        sem_t sem = {};

    public:
        semaphore(int val = 1) { sem_init(&sem, 0, val); }
        semaphore(const semaphore&) = delete;
        semaphore(semaphore&&) = delete;

        ~semaphore() { sem_destroy(&sem); }

        void post() { sem_post(&sem); }
        void wait() { sem_wait(&sem); }
        void trywait() { sem_trywait(&sem); }
        int getvalue() { int val; sem_getvalue(&sem, &val); return val; }
    };
}
#endif

#endif
