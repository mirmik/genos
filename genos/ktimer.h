#ifndef GENOS_TIMER_TASKLET_H
#define GENOS_TIMER_TASKLET_H

#include <igris/osinter/ctrobj.h>
#include <igris/sync/syslock.h>
#include <igris/time/systime.h>

extern struct dlist_head ktimer_list;

namespace genos
{
    class ktimer;
}

typedef void (*ktimer_callback_t)(void *arg, genos::ktimer *tim);

namespace genos
{
    class ktimer
    {
    public:
        struct ctrobj ctr = CTROBJ_DECLARE(ctr, CTROBJ_KTIMER_DELEGATE);
        int64_t start = 0;
        int64_t interval = 0;
        ktimer_callback_t act = {};
        void *arg = nullptr;

    public:
        ktimer() = default;

        ktimer(ktimer_callback_t act, void *arg, int64_t interval)
            : interval(interval), act(act), arg(arg)
        {
        }

        ktimer(ktimer_callback_t act,
               void *arg,
               int64_t start,
               int64_t interval)
            : start(start), interval(interval), act(act), arg(arg)
        {
        }

        void
        init(ktimer_callback_t act, void *arg, int64_t start, int64_t interval)
        {
            this->start = start;
            this->interval = interval;
            this->act = act;
            this->arg = arg;
        }

        void plan();

        void swift()
        {
            start += interval;
        }

        void replan()
        {
            swift();
            plan();
        }

        bool planned();
        void unplan();
        uint8_t check(int64_t ms);

        void set_start_now();
        void set_interval_ms(int64_t t);

        void deinit()
        {
            dlist_del_init(&ctr.lnk);
        }
    };

    void ktimer_manager_step();
    void ktimer_manager_step(int64_t curtime); // < for testing
    size_t ktimer_manager_planed_count();
}

#endif