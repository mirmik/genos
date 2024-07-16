#ifndef IGRIS_TIME_TIMER_MANAGER_H
#define IGRIS_TIME_TIMER_MANAGER_H

#include <igris/container/dlist.h>
#include <igris/event/delegate.h>
#include <igris/sync/syslock.h>
#include <igris/time/systime.h>

#include <algorithm>
#include <functional>
#include <tuple>

namespace igris
{
    template <class Time_t,
              class Difftime_t =
                  decltype(std::declval<Time_t>() - std::declval<Time_t>())>
    class timer_spec
    {
    public:
        using time_t = Time_t;
        using difftime_t = Difftime_t;
    };

    template <typename TimeSpec> class timer_manager_basic;
    template <typename TimeSpec = timer_spec<int64_t>> class timer_head_basic
    {
        using time_t = typename TimeSpec::time_t;
        using difftime_t = typename TimeSpec::difftime_t;

        friend class timer_manager_basic<TimeSpec>;
        dlist_node lnk = {};
        time_t _start = 0;
        difftime_t _interval = 0;

    public:
        bool is_planned()
        {
            return lnk.is_linked();
        }
        void unplan()
        {
            lnk.unlink();
        }
        virtual void execute() = 0;
        virtual ~timer_head_basic() = default;

        time_t finish() const
        {
            return _start + _interval;
        }
        bool check(time_t curtime)
        {
            return curtime - _start >= _interval;
        }
        void set_start(time_t t)
        {
            _start = t;
        }
        void set_interval(difftime_t t)
        {
            _interval = t;
        }
        void shift()
        {
            _start += _interval;
        }
    };

    template <typename TimeSpec, typename... Args>
    class timer_basic : public timer_head_basic<TimeSpec>
    {
        igris::delegate<void, Args...> dlg;
        std::tuple<Args...> args;

    public:
        timer_basic(igris::delegate<void, Args...> dlg, Args &&...args)
            : dlg(dlg), args(std::forward<Args>(args)...)
        {
        }

        void execute() override
        {
            std::apply(dlg, args);
        }
    };

    template <typename TimeSpec> class timer_manager_basic
    {
        using time_t = typename TimeSpec::time_t;
        using difftime_t = typename TimeSpec::difftime_t;

        igris::dlist<timer_head_basic<TimeSpec>,
                     &timer_head_basic<TimeSpec>::lnk>
            timer_list = {};

    public:
        timer_manager_basic() = default;

        void plan(timer_head_basic<TimeSpec> &tim)
        {
            auto finish = tim.finish();

            system_lock();
            tim.unplan();

            auto it = std::find_if(timer_list.begin(),
                                   timer_list.end(),
                                   [&](const auto &tim)
                                   { return finish < tim.finish(); });

            timer_list.move_prev(tim, it);
            system_unlock();
        }

        void
        plan(timer_head_basic<TimeSpec> &tim, time_t start, difftime_t interval)
        {
            tim.set_start(start);
            tim.set_interval(interval);
            plan(tim);
        }

        void exec(time_t curtime)
        {
            system_lock();

            while (!timer_list.empty())
            {
                auto &tim = timer_list.first();
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

        difftime_t minimal_interval(time_t curtime)
        {
            return timer_list.first().finish() - curtime;
        }
    };

    template <typename... Args>
    using timer = timer_basic<timer_spec<int64_t>, Args...>;

    using timer_head = timer_head_basic<timer_spec<int64_t>>;

    using timer_manager = timer_manager_basic<timer_spec<int64_t>>;
}

#endif
