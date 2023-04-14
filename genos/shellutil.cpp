#include <genos/schedee.h>
#include <genos/schedee_api.h>
#include <genos/shellutil.h>
#include <igris/time/systime.h>
#include <nos/print.h>

int info_cmd(const nos::argv &args, nos::ostream &os)
{
    for (auto &sch : genos::schedee_list)
    {
        nos::print_to(os, " mnemo: ");
        nos::print_to(os, sch.mnemo());
        nos::print_to(os, " pid: ");
        nos::print_to(os, std::to_string(sch.pid));
        nos::print_to(os, " state: ");
        nos::print_to(os, schedee_state_to_sting(sch.sch_state));
        nos::print_to(os, " prio: ");
        nos::print_to(os, std::to_string(sch.prio));
        nos::print_to(os, " counter: ");
        nos::print_to(os, std::to_string(sch._execute_counter));
        nos::println_to(os);
    }
    return 0;
}

int time_cmd(const nos::argv &args, nos::ostream &os)
{
    uint64_t time = igris::micros();
    nos::println_to(os, "time: ", time);
    return 0;
}

int execmon_cmd(const nos::argv &args, nos::ostream &os)
{
    auto str = genos::execution_monitor_manager::summary();
    nos::print_to(os, str);
    return 0;
}

nos::executor genos::schedee_manager_executor{
    {nos::command{"ps", "proccess information", info_cmd},
     nos::command{"time", "time information", time_cmd},
     nos::command{"execmon", "execution monitor information", monitor_cmd}}};
