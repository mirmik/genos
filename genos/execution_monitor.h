#ifndef EXECUTION_CONTROLLER_H
#define EXECUTION_CONTROLLER_H

#include <igris/container/dlist.h>
#include <igris/time/systime.h>
#include <stdint.h>

namespace genos
{
    class execution_monitor
    {
        const char *_mnemo = "undefined";
        uint64_t _summary_time = 0;
        uint32_t _summary_count = 0;
        uint64_t _last_session_time = 0;
        uint64_t _start_time = 0;

    public:
        igris::dlist_node _lnk = {};

    public:
        execution_monitor() = default;
        execution_monitor(const char *mnemo);

        const char *mnemo() const
        {
            return _mnemo;
        }
        uint64_t summary_time() const
        {
            return _summary_time;
        }
        uint32_t summary_count() const
        {
            return _summary_count;
        }
        uint64_t last_session_time() const
        {
            return _last_session_time;
        }
        uint64_t start_time() const
        {
            return _start_time;
        }

        void start_session()
        {
            _start_time = igris::micros();
        }

        void end_session()
        {
            _last_session_time = igris::micros() - _start_time;
            _summary_time += _last_session_time;
            _summary_count++;
        }

        void set_mnemo(const char *name)
        {
            _mnemo = name;
        }
    };

    class execution_monitor_manager
    {
        static igris::dlist<execution_monitor, &execution_monitor::_lnk> _list;

    public:
        static void register_controller(execution_monitor *ctrl)
        {
            _list.move_back(*ctrl);
        }

        static std::string summary();

        static size_t count()
        {
            return _list.size();
        }
    };
}

#endif