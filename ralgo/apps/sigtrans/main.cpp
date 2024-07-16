#include <chrono>
#include <crow/address.h>
#include <crow/gates/udpgate.h>
#include <crow/nodes/publisher_node.h>
#include <crow/tower.h>
#include <errno.h>
#include <fcntl.h>
#include <igris/getopt/cliopts.h>
#include <igris/time/systime.h>
#include <igris/util/string.h>
#include <iostream>
#include <memory>
#include <nos/fprint.h>
#include <nos/input.h>
#include <nos/io/file.h>
#include <nos/print.h>
#include <ralgo/heimer/command.h>
#include <ralgo/heimer/executor.h>
#include <signal.h>
#include <string>
#include <thread>
#include <unistd.h>

using std::chrono::operator""us;

int DEBUG = 0;

std::unique_ptr<std::thread> execute_thread;
std::unique_ptr<std::thread> fast_execute_thread;
int started = 0;
int cancel_token = 0;

crow::udpgate udpgate;
crow::hostaddr crowaddr;

namespace heimer
{
    double fast_cycle_frequence()
    {
        return 10000;
    }
}

void execute_routine()
{
    auto target_time = std::chrono::steady_clock::now();

    while (1)
    {
        if (cancel_token)
            return;
        heimer::executor.execute_if_allowed(discrete_time());
        std::this_thread::sleep_until(target_time += 10000us);
    }
}

void fast_execute_routine()
{
    auto target_time = std::chrono::steady_clock::now();

    while (1)
    {
        heimer::executor.exec_fast_cycle();

        if (cancel_token)
            return;
        std::this_thread::sleep_until(target_time += 100us);
    }
}

void start_routine()
{
    if (started)
    {
        nos::println("It has start early");
        return;
    }

    started = 1;
    cancel_token = 0;

    heimer::executor.allocate_order_table(heimer::signal_processors_count());

    heimer::signal_processor *proc;
    dlist_for_each_entry(proc, &heimer::signal_processor_list, list_lnk)
    {
        heimer::executor.append_processor(proc);
    }
    heimer::executor.order_sort();
    // heimer::executor.notification_prepare("sigtrans/feedpos", crowaddr);

    execute_thread.reset(new std::thread(execute_routine));
    fast_execute_thread.reset(new std::thread(fast_execute_routine));
}

void stop_routine()
{
    if (!started)
    {
        nos::println("Is not started yet");
        return;
    }

    started = 0;
    cancel_token = 1;
    execute_thread->join();
}

void execinfo()
{
    if (!started)
    {
        nos::println("Is not started yet");
        return;
    }

    for (int i = 0; i < heimer::executor.order_table_size; ++i)
    {
        heimer::signal_processor *proc = heimer::executor.order_table[i];
        nos::println(
            std::string_view(proc->name().data(), proc->name().size()));
    }
}

int exec(const std::string &line)
{
    int sts;
    char output[512];
    memset(output, 0, 512);

    if (line.size() == 0)
        return 0;

    if (igris::trim(line) == "execinfo")
    {
        execinfo();
        return 0;
    }

    if (igris::trim(line) == "time")
    {
        nos::println(discrete_time(), discrete_time_frequency());
        return 0;
    }

    int ret;
    sts = heimer::command_exec_safe(line.c_str(), output, 512, &ret);

    if (sts == ENOENT)
    {
        nos::println("Command not found.");
        return -1;
    }

    else
    {
        if (DEBUG)
        {
            nos::fprintln("({}) : ", ret);
        }

        nos::print(output);
        return ret;
    }
}

void sigint_handler(int)
{
    quick_exit(0);
}

int main(int argc, char **argv)
{
    signal(SIGINT, &sigint_handler);
    crowaddr = crow::crowker_address();
    if (crowaddr.size() == 0)
    {
        nos::println("Crowker address has zero size");
        exit(0);
    }

    igris::cliopts cli;
    cli.add_option("debug", 'd');
    cli.add_option("crowdiag", 'D');
    cli.add_string("script", 's', "");
    cli.parse(argc, argv);

    if (cli.get_option("crowdiag"))
        crow::diagnostic_setup(true);

    DEBUG = cli.get_option("debug");
    auto script_path = cli.get_string("script").unwrap();

    if (script_path.size())
    {
        std::string str;
        nos::file fl(script_path.c_str(), O_RDONLY);

        if (!fl.good())
        {
            nos::println("Script file is not exists?");
            exit(0);
        }

        while ((str = *nos::readline_from(fl, 1024, false)).size())
        {
            str = igris::trim(str);

            if (str[0] == '/')
                continue;

            nos::println("script:", str);
            int sts = exec(str);
            if (sts)
            {
                nos::println();
                exit(0);
            }
        }
    }

    udpgate.open();
    udpgate.bind(CROW_UDPGATE_NO);
    crow::start_spin();

    start_routine();

    while (1)
    {
        std::string line;
        std::getline(std::cin, line);

        exec(line);
    }

    crow::stop_spin();
}
