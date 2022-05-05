#include <genos/ktimer.h>
#include <genos/schedee.h>
#include <igris/dprint.h>
#include <igris/time/jiffies-systime.h>
#include <stdlib.h>

DLIST_HEAD(ktimer_list);

/*void ktimer_base_init(struct ktimer_base *tim, int64_t start, int64_t interval,
                      uint8_t ctrtype)
{
    ctrobj_init(&tim->ctr, ctrtype);
    tim->start = start;
    tim->interval = interval;
}

void ktimer_init(genos::ktimer *tim, ktimer_callback_t act, void *arg,
                 int64_t start, int64_t interval)
{
    ktimer_base_init(&tim->tim, start, interval, CTROBJ_KTIMER_DELEGATE);
    tim->act = act;
    tim->arg = arg;
}
*/
uint8_t genos::ktimer_base::check(int64_t now)
{
    return now - start >= interval;
}
/*
void ktimer_base_init_for_milliseconds(struct ktimer_base *tim,
                                       uint32_t interval, uint8_t ctrtype)
{
    ktimer_base_init(tim, jiffies(), ms2jiffies(interval), ctrtype);
}

void ktimer_init_for_milliseconds(genos::ktimer *tim, ktimer_callback_t act,
                                  void *arg, uint32_t interval)
{
    ktimer_init(tim, act, arg, jiffies(), ms2jiffies(interval));
}*/

void genos::ktimer_base::set_start_now() { start = jiffies(); }
void genos::ktimer_base::set_interval_ms(int64_t t) { interval = ms2jiffies(t); }

/*void ktimer_list_debug_print()
{
    struct ktimer_base *it;
    dlist_for_each_entry(it, &ktimer_list, ctr.lnk)
    {
        dprln("timer", it->start, it->interval);
    }
}*/

bool genos::ktimer_base::planned() { return ctr.lnk.next != ctr.lnk.prev; }

void genos::ktimer_base::unplan() { dlist_del_init(&ctr.lnk); }

void genos::ktimer_base::plan()
{
    struct ktimer_base *it;
    struct dlist_head *sit = nullptr;
    int64_t it_final;
    int64_t final;

    final = start + interval;
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

    dlist_add_tail(&ctr.lnk, sit ? sit : &ktimer_list);

    system_unlock();
}

void ktimer_execute(genos::ktimer_base *tim)
{
    switch (tim->ctr.type)
    {
    case CTROBJ_KTIMER_DELEGATE:
    {
        genos::ktimer *t = reinterpret_cast<genos::ktimer*>(tim);
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

void ktimer_manager_step()
{
    int64_t now;

    now = jiffies();

    system_lock();

    while (!dlist_empty(&ktimer_list))
    {
        genos::ktimer_base *it =
            dlist_first_entry(&ktimer_list, genos::ktimer_base, ctr.lnk);
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