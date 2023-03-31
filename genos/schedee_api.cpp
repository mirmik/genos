#include <genos/displace.h>
#include <genos/ktimer.h>
#include <genos/schedee.h>
#include <genos/schedee_api.h>

#include <igris/sync/syslock.h>
#include <igris/util/bug.h>

using namespace genos;

int genos::wait_for_avail(unsigned int fd)
{

    genos::resource_table &resources =
        genos::current_schedee()->resource_table();
    if (resources.size() <= fd)
        return -1;
    auto &openr = resources[fd];
    return openr.wait_for_avail();
}

void genos::current_schedee_exit()
{
    genos::schedee *sch = current_schedee();
    __schedee_final(sch);
    sch->displace();
}

int genos::current_schedee_displace()
{
    genos::schedee *sch = current_schedee();
    sch->syslock_counter_save = syslock_counter();
    return sch->displace();
}

void __timer_schedee_unsleep(void *priv, genos::ktimer *tim)
{
    (void)tim;
    genos::schedee *sch = (genos::schedee *)priv;
    schedee_start(sch);
}

int genos::current_schedee_msleep(unsigned int ms, int64_t start)
{
    current_schedee_msleep_without_displace(ms, start);
    int re = current_schedee_displace();
    return re;
}

int genos::current_schedee_msleep(unsigned int ms)
{
    return genos::current_schedee_msleep(ms, igris::system_time());
}

void genos::current_schedee_msleep_without_displace(unsigned int ms,
                                                    int64_t start)
{
    genos::schedee *sch;
    genos::ktimer *timer;

    sch = current_schedee();

    if (sch == NULL)
    {
        return;
    }

    timer = &sch->ktimer;

    system_lock();
    dlist_del_init(&sch->ctr.lnk);
    system_unlock();

    sch->ctr.type = CTROBJ_KTIMER_SCHEDEE;
    sch->sch_state = schedee_state::wait;

    *timer = genos::ktimer(__timer_schedee_unsleep,
                           (void *)sch,
                           start, // start
                           ms     // interval
    );

    timer->plan();
}

void genos::current_schedee_msleep_without_displace(unsigned int ms)
{
    genos::current_schedee_msleep_without_displace(ms, igris::system_time());
}