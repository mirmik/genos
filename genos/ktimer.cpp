#include <genos/ktimer.h>
#include <genos/schedee.h>
#include <igris/dprint.h>
#include <igris/time/jiffies-systime.h>
#include <igris/time/systime.h>
#include <stdlib.h>

igris::dlist<genos::ktimer, &genos::ktimer::lnk> ktimer_list;

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
    return lnk.next_node() != lnk.prev_node();
}

void genos::ktimer::unplan()
{
    lnk.unlink();
}

void genos::ktimer::plan()
{
    int64_t final = start + interval;

    system_lock();
    unplan();

    auto it = ktimer_list.begin();
    for (; it != ktimer_list.end(); ++it)
    {
        int64_t it_final = it->start + it->interval;
        if (final - it_final < 0)
            break;
    }
    ktimer_list.move_prev(*this, it);

    system_unlock();
}

void ktimer_execute(genos::ktimer *tim)
{
    tim->lnk.unlink();
    tim->act(tim->arg, tim);
}

void genos::ktimer_manager_step(int64_t now)
{
    system_lock();

    while (!ktimer_list.empty())
    {
        genos::ktimer *it = &ktimer_list.first();
        system_unlock();

        if (it->check(now))
        {
            ktimer_execute(it);
        }
        else
        {
            return;
        }

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
    return ktimer_list.size();
}

void genos::ktimer_manager_init()
{
    // pass
}