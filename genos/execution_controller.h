#ifndef EXECUTION_CONTROLLER_H
#define EXECUTION_CONTROLLER_H

#include <igris/container/dlist.h>
#include <igris/time/systime.h>
#include <stdint.h>

namespace genos
{
    class execution_controller
    {
        const char *_mnemo = "undefined";
        uint64_t _summary_time = 0;
        uint32_t _summary_count = 0;
        uint64_t _last_session_time = 0;
        uint64_t _start_time = 0;

    public:
        igris::dlist_node _lnk = {};

    public:
        execution_controller(const char *mnemo);

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
    };

    class execution_controller_manager
    {
        static igris::dlist<execution_controller, &execution_controller::_lnk>
            _list;

    public:
        static void register_controller(execution_controller *ctrl)
        {
            _list.move_back(*ctrl);
        }

        static void print_summary();

        static size_t count()
        {
            return _list.size();
        }
    };
}

#endif