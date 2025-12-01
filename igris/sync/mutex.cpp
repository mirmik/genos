#include <errno.h>

#include <igris/container/dlist.h>
#include <igris/osinter/wait.h>
#include <igris/sync/mutex.h>
#include <igris/sync/syslock.h>

void igris_mutex_init(struct igris_mutex *mtx)
{
    dlist_init(&mtx->wait_list);
    mtx->locked = 0;
}

int igris_mutex_lock(struct igris_mutex *mtx)
{
    int sts;
    void *future;

    system_lock();

    while (1)
    {
        if (!mtx->locked)
        {
            mtx->locked = 1;
            system_unlock();
            return 0;
        }

        system_unlock();
        sts = wait_current_schedee(
            reinterpret_cast<igris::dlist_base *>(&mtx->wait_list), 0, &future);
        if (sts)
            return sts;
        system_lock();
    }
}

int igris_mutex_trylock(struct igris_mutex *mtx)
{
    int ret = -EBUSY;

    system_lock();

    if (!mtx->locked)
    {
        mtx->locked = 1;
        ret = 0;
    }

    system_unlock();

    return ret;
}

int igris_mutex_unlock(struct igris_mutex *mtx)
{
    system_lock();

    if (!mtx->locked)
    {
        system_unlock();
        return -EINVAL;
    }

    mtx->locked = 0;
    if (!dlist_empty(&mtx->wait_list))
    {
        unwait_one(reinterpret_cast<igris::dlist_base *>(&mtx->wait_list), 0);
    }

    system_unlock();

    return 0;
}

int igris_mutex_destroy(struct igris_mutex *mtx)
{
    int busy;

    system_lock();
    busy = mtx->locked || !dlist_empty(&mtx->wait_list);
    system_unlock();

    return busy ? -EBUSY : 0;
}
