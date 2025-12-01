#include <errno.h>

#include <igris/container/dlist.h>
#include <igris/osinter/wait.h>
#include <igris/sync/semaphore.h>

void igris_sem_init(struct igris_semaphore *sem, int shared, int val)
{
    (void)shared;
    sem->count = val;
    dlist_init(&sem->wait_list);
}

int igris_sem_wait(struct igris_semaphore *sem)
{
    int sts;
    void *_;
    system_lock();

    while (1)
    {
        if (sem->count > 0)
        {
            --sem->count;
            system_unlock();
            break;
        }

        system_unlock();
        sts = wait_current_schedee(
            reinterpret_cast<igris::dlist_base *>(&sem->wait_list), 0, &_);
        if (sts)
            return sts;
        system_lock();
    }
    return 0;
}

int igris_sem_trywait(struct igris_semaphore *sem)
{
    int status = -1;

    system_lock();

    if (sem->count > 0)
    {
        --sem->count;
        status = 0;
    }

    system_unlock();
    return status;
}

int igris_sem_post(struct igris_semaphore *sem)
{
    system_lock();

    ++sem->count;
    if (!dlist_empty(&sem->wait_list))
        unwait_one(reinterpret_cast<igris::dlist_base *>(&sem->wait_list), 0);

    system_unlock();
    return 0;
}

int igris_sem_getvalue(struct igris_semaphore *sem, int *val)
{
    system_lock();
    *val = sem->count;
    system_unlock();
    return 0;
}

int igris_sem_destroy(struct igris_semaphore *sem)
{
    int busy;

    system_lock();
    busy = !dlist_empty(&sem->wait_list);
    dlist_init(&sem->wait_list);
    sem->count = 0;
    system_unlock();

    return busy ? -EBUSY : 0;
}
