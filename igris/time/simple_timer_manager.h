#ifndef IGRIS_SIMPLE_TIME_TIMER_MANAGER_H
#define IGRIS_SIMPLE_TIME_TIMER_MANAGER_H

#include <algorithm>
#include <functional>
#include <igris/container/dlist.h>
#include <igris/event/delegate.h>
#include <igris/sync/syslock.h>
#include <igris/time/systime.h>
#include <tuple>

namespace igris
{
    class timer_manager;

    class timer_head
    {
    protected:
        int64_t _start = 0;
        int64_t _interval = 0;

    public:
        int64_t finish()
        {
            return _start + _interval;
        }
        bool check(int64_t curtime)
        {
            return curtime - _start >= _interval;
        }
        void set_start(int64_t t)
        {
            _start = t;
        }
        void set_interval(int64_t t)
        {
            _interval = t;
        }
        void shift()
        {
            _start += _interval;
        }
        virtual ~timer_head() = default;
    };

    class managed_timer_head : public timer_head
    {
        friend class timer_manager;
        dlist_head lnk = DLIST_HEAD_INIT(lnk);

    public:
        bool is_planned()
        {
            return lnk.next != &lnk && &lnk != lnk.prev;
        }
        void unplan()
        {
            dlist_del_init(&lnk);
        }
        virtual void execute() = 0;
        virtual ~managed_timer_head() = default;
    };

    template <typename... Args> class timer : public managed_timer_head
    {
        igris::delegate<void, Args...> dlg;
        std::tuple<Args...> args;

    public:
        timer(igris::delegate<void, Args...> dlg, Args &&... args)
            : dlg(dlg), args(std::forward<Args>(args)...)
        {
        }

        void execute() override
        {
            std::apply(dlg, args);
        }
    };

    class timer_manager
    {
        igris::dlist<managed_timer_head, &managed_timer_head::lnk> timer_list =
            {};

    public:
        timer_manager() = default;

        void plan(managed_timer_head &tim)
        {
            auto final = tim.finish();

            system_lock();
            tim.unplan();

            auto it = std::find_if(
                timer_list.begin(), timer_list.end(), [final](auto &tim) {
                    return tim.is_final_after(final);
                });

            timer_list.move_prev(tim, it);
            system_unlock();
        }

        void plan(managed_timer_head &tim, int64_t start, int64_t interval)
        {
            tim.set_start(start);
            tim.set_interval(interval);
            plan(tim);
        }

        void exec(int64_t curtime)
        {
            system_lock();

            while (!timer_list.empty())
            {
                managed_timer_head &tim = timer_list.first();
                system_unlock();

                if (tim.check(curtime))
                {
                    tim.execute();
                    auto linked = tim.is_planned();
                    if (linked)
                    {
                        tim.unplan();
                        tim.shift();
                        plan(tim);
                    }
                }
                else
                    return;

                system_lock();
            }

            system_unlock();
        }

        bool empty() const
        {
            return timer_list.empty();
        }

        int64_t minimal_interval(int64_t curtime)
        {
            return timer_list.first().finish() - curtime;
        }
    };
}

#endif
