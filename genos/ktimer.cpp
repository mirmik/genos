#include <genos/ktimer.h>
#include <genos/schedee.h>
#include <igris/dprint.h>
#include <igris/time/jiffies-systime.h>
#include <igris/time/systime.h>
#include <stdlib.h>

DLIST_HEAD(ktimer_list);

uint8_t genos::ktimer::check(int64_t now)
{
    return now - start >= interval;
}

void genos::ktimer::set_start_now()
{
    start = jiffies();
}
void genos::ktimer::set_interval_ms(int64_t t)
{
    interval = ms2jiffies(t);
}

bool genos::ktimer::planned()
{
    return ctr.lnk.next != ctr.lnk.prev;
}

void genos::ktimer::unplan()
{
    dlist_del_init(&ctr.lnk);
}

void genos::ktimer::plan()
{
    struct ktimer *it;
    struct dlist_head *sit = nullptr;
    int64_t it_final;
    int64_t final;

    final = start + interval;
    sit = NULL;

    system_lock();
    unplan();

    dlist_for_each_entry(it, &ktimer_list, ctr.lnk)
    {
        it_final = it->start + it->interval;

        if (final - it_final < 0)
        {
            sit = &it->ctr.lnk;
            break;
        }
    }

    dlist_add_tail(&ctr.lnk, sit ? sit : &ktimer_list);

    system_unlock();
}

void ktimer_execute(genos::ktimer *tim)
{
    switch (tim->ctr.type)
    {
    case CTROBJ_KTIMER_DELEGATE:
    {
        genos::ktimer *t = reinterpret_cast<genos::ktimer *>(tim);
        dlist_del_init(&tim->ctr.lnk);
        t->act(t->arg, t);
        break;
    }

    case CTROBJ_KTIMER_SCHEDEE:
    {
        genos::schedee *sch = mcast_out(tim, genos::schedee, ktimer);
        // извлечение из списка произойдёт при запуске, т.к. waiter использует
        // поле schedee листа.
        schedee_start(sch);
        break;
    }

    default:
        break;
    }
}

void genos::ktimer_manager_step(int64_t now)
{
    system_lock();

    while (!dlist_empty(&ktimer_list))
    {
        genos::ktimer *it =
            dlist_first_entry(&ktimer_list, genos::ktimer, ctr.lnk);
        system_unlock();

        if (it->check(now))
        {
            ktimer_execute(it);
        }
        else
            return;

        system_lock();
    }

    system_unlock();
}

void genos::ktimer_manager_step()
{
    int64_t now = igris::system_time();
    ktimer_manager_step(now);
}

size_t genos::ktimer_manager_planed_count()
{
    return dlist_size(&ktimer_list);
}

void ktimer_manager_init()
{
    dlist_init(&ktimer_list);
}