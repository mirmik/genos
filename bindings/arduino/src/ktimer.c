#include <genos/ktimer.h>
#include <igris/sync/syslock.h>

static DLIST_HEAD(ktimer_list);

int ktimer_check(struct ktimer_head *timer, uint64_t curtime)
{
    return curtime - timer->start >= timer->interval;
}

void ktimer_init(struct ktimer_head *timer, ktimer_callback_t callback,
                 void *privdata, uint64_t start, int64_t interval)
{
    timer->callback = callback;
    timer->privdata = privdata;
    timer->start = start;
    timer->interval = interval;
}

void ktimer_swift(struct ktimer_head *timer)
{
    timer->start += timer->interval;
}

uint64_t ktimer_finish(struct ktimer_head *timer)
{
    return timer->start + timer->interval;
}

void ktimer_restart(struct ktimer_head *timer, uint64_t start)
{
    timer->start = start;
    ktimer_plan(timer);
}

bool ktimer_planned(struct ktimer_head *timer)
{
    return timer->ctr.lnk.next != timer->ctr.lnk.prev;
}

void ktimer_unplan(struct ktimer_head *timer)
{
    dlist_del_init(&timer->ctr.lnk);
}

void ktimer_plan(struct ktimer_head *tim)
{
    struct ktimer_head *it;
    struct dlist_head *sit = NULL;
    int64_t it_final;
    int64_t final;

    final = tim->start + tim->interval;
    sit = NULL;

    system_lock();

    dlist_for_each_entry(it, &ktimer_list, ctr.lnk)
    {
        it_final = it->start + it->interval;

        if (final - it_final < 0)
        {
            sit = &it->ctr.lnk;
            break;
        }
    }

    dlist_add_tail(&tim->ctr.lnk, sit ? sit : &ktimer_list);

    system_unlock();
}

void ktimer_execute(struct ktimer_head *tim)
{
    dlist_del_init(&tim->ctr.lnk);
    tim->callback(tim->privdata, tim);
}

void ktimer_manager_step(uint64_t curtime)
{
    system_lock();

    while (!dlist_empty(&ktimer_list))
    {
        ktimer_t *it = dlist_first_entry(&ktimer_list, ktimer_t, ctr.lnk);
        system_unlock();

        if (ktimer_check(it, curtime))
        {
            ktimer_execute(it);
        }
        else
            return;

        system_lock();
    }

    system_unlock();
}