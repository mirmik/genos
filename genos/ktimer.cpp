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
    system_lock();
    bool result = lnk.is_linked();
    system_unlock();
    return result;
}

void genos::ktimer::unplan()
{
    system_lock();
    lnk.unlink();
    system_unlock();
}

void genos::ktimer::plan()
{
    int64_t final = start + interval;

    system_lock();
    lnk.unlink();

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

void genos::ktimer_manager_step(int64_t now)
{
    while (true)
    {
        system_lock();

        if (ktimer_list.empty())
        {
            system_unlock();
            return;
        }

        genos::ktimer *tim = &ktimer_list.first();
        if (!tim->check(now))
        {
            system_unlock();
            return;
        }

        tim->lnk.unlink();
        system_unlock();

        tim->act(tim->arg, tim);
    }
}

void genos::ktimer_manager_step()
{
    int64_t now = igris::system_time();
    ktimer_manager_step(now);
}

size_t genos::ktimer_manager_planed_count()
{
    system_lock();
    size_t count = ktimer_list.size();
    system_unlock();
    return count;
}

void genos::ktimer_manager_init()
{
    system_lock();
    ktimer_list.clear();
    system_unlock();
}
