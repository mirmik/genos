#ifndef GENOS_TIMER_TASKLET_H
#define GENOS_TIMER_TASKLET_H

#include <igris/osinter/ctrobj.h>
#include <igris/sync/syslock.h>
#include <igris/time/systime.h>

extern struct dlist_head ktimer_list;

namespace genos
{
    struct ktimer;
}

typedef void (*ktimer_callback_t)(void *arg, genos::ktimer *tim);

namespace genos
{
    class ktimer_base
    {
    public:
        struct ctrobj ctr;

        int64_t start;
        int64_t interval;

    public:
        ktimer_base() : ctr(CTROBJ_DECLARE(ctr, CTROBJ_KTIMER_DELEGATE)) {}

        ktimer_base(int64_t start, int64_t interval)
            : ctr(CTROBJ_DECLARE(ctr, CTROBJ_KTIMER_DELEGATE)), start(start),
              interval(interval)
        {
        }

        void init(int64_t start, int64_t interval)
        {
            this->start=start;
            this->interval=interval;
        }

        ktimer_base(const ktimer_base &) = default;
        ktimer_base &operator=(const ktimer_base &) = default;

        void plan();

        void swift() { start += interval; }

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
    };

    class ktimer : public ktimer_base
    {
    public:
        ktimer_callback_t act;
        void *arg;

    public:
        ktimer() = default;

        ktimer(ktimer_callback_t act, void *arg, int64_t interval)
            : ktimer_base(0, interval), act(act), arg(arg)
        {
        }

        ktimer(ktimer_callback_t act, void *arg, int64_t start,
               int64_t interval)
            : ktimer_base(start, interval), act(act), arg(arg)
        {
        }

        void init(ktimer_callback_t act, void *arg, int64_t start, int64_t interval)
        {
            ktimer_base::init(start, interval);
            this->act = act; 
            this->arg = arg;
        }
    };
}

__BEGIN_DECLS

void ktimer_manager_step();

void ktimer_init_for_milliseconds(genos::ktimer *tim, ktimer_callback_t act,
                                  void *arg, uint32_t ms);
void ktimer_base_init_for_milliseconds(genos::ktimer_base *tim,
                                       uint32_t interval, uint8_t ctrtype);

//void ktimer_list_debug_print();

__END_DECLS

#endif