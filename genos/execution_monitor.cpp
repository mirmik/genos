#include <genos/execution_monitor.h>

__attribute__((init_priority(101)))
igris::dlist<genos::execution_monitor, &genos::execution_monitor::_lnk>
    genos::execution_monitor_manager::_list;

genos::execution_monitor::execution_monitor(const char *mnemo)
{
    _mnemo = mnemo;
    genos::execution_monitor_manager::register_controller(this);
}

std::string genos::execution_monitor_manager::summary()
{
    std::string str;
    for (auto &ctrl : _list)
    {
        str += ctrl.mnemo();
        str += " sum:";
        str += std::to_string(ctrl.summary_time());
        str += " cnt:";
        str += std::to_string(ctrl.summary_count());
        str += " last:";
        str += std::to_string(ctrl.last_session_time());
        str += "\r\n";
    }
    return str;
}