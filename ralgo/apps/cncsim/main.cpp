#include <chrono>
#include <crow/addons/logger.h>
#include <crow/address.h>
#include <crow/gates/udpgate.h>
#include <crow/nodes/publisher_node.h>
#include <crow/nodes/service_node.h>
#include <crow/tower.h>
#include <getopt.h>
#include <igris/time/systime.h>
#include <mutex>
#include <nos/input.h>
#include <nos/io/string_writer.h>
#include <nos/shell/argv.h>
#include <ralgo/cnc/feedback_guard.h>
#include <ralgo/cnc/interpreter.h>
#include <ralgo/cnc/planner.h>
#include <ralgo/cnc/revolver.h>
#include <ralgo/cnc/themes.h>
#include <ralgo/robo/stepper.h>
#include <thread>

igris::ring<cnc::planner_block> blocks{40};
igris::ring<cnc::control_shift> shifts{400};
cnc::revolver revolver;
cnc::planner planner(&blocks, &revolver);
cnc::feedback_guard feedback_guard(3);
cnc::interpreter interpreter(&blocks, &planner, &revolver, &feedback_guard);
robo::stepper steppers[3];
robo::stepper *steppers_ptrs[] = {&steppers[0], &steppers[1], &steppers[2]};
std::mutex mtx;

void planner_thread_function();
void revolver_thread_function();
void telemetry_thread_function();
std::thread revolver_thread;
std::thread planner_thread;
std::thread telemetry_thread;

auto crowker = crow::crowker_address();

using std::chrono::operator""ms;
using std::chrono::operator""us;

crow::publisher_node publisher(crowker, "cncsim" RALGO_CNC_POSES_BIN_THEME);
crow::publisher_node publisher_txt(crowker, "cncsim" RALGO_CNC_POSES_THEME);
crow::publisher_node publisher_log(crowker, "cncsim" RALGO_CNC_LOG_THEME);
crow::publish_logger logger("ralgo", &publisher_log);
crow::service_node control_service(
    crowker,
    "cncsim" RALGO_CNC_CLI_SERVICE,
    +[](char *cmd, int len, crow::service_node &srv) {
        std::lock_guard<std::mutex> lock(mtx);
        cmd[len] = 0;
        nos::println("input: ", std::string(cmd, len), "END");
        nos::string_buffer answer;
        interpreter.executor.execute(nos::tokens(cmd), answer);
        srv.reply(answer.str().data(), answer.str().size());
    });

auto now()
{
    return std::chrono::steady_clock::now();
}

void planner_thread_function()
{
    auto awake = now();
    while (1)
    {
        awake += 1ms;
        std::this_thread::sleep_until(awake);
        mtx.lock();
        planner.serve();
        mtx.unlock();
    }
}

void telemetry_thread_function()
{
    int64_t poses[NMAX_AXES];
    auto awake = now();
    while (1)
    {
        awake += 10ms;
        std::this_thread::sleep_until(awake);
        for (size_t i = 0; i < interpreter.get_axes_count(); ++i)
            poses[i] = int64_t(steppers[i].steps_count());
        publisher.publish(
            nos::buffer(poses, sizeof(int64_t) * interpreter.get_axes_count()));

        std::string poses_str = "";
        for (size_t i = 0; i < interpreter.get_axes_count(); ++i)
            poses_str += std::to_string(poses[i]) + " ";
        publisher_txt.publish(nos::buffer(poses_str.data(), poses_str.size()));
    }
}

namespace heimer
{
    cnc_float_type fast_cycle_frequence()
    {
        return 5000;
    }
}

void revolver_thread_function()
{
    auto awake = now();
    while (1)
    {
        awake += 200us;
        std::this_thread::sleep_until(awake);

        mtx.lock();
        revolver.serve();
        mtx.unlock();
    }
}

void print_help()
{
    printf("Usage: crowker [OPTION]...\n"
           "\n"
           "Common option list:\n"
           "  -h, --help            print this page\n"
           "  -p, --noprotect            disable global protection\n"
           "Crow services:\n"
           "	cncsim/cli - cncsim control\n"
           "Crow topics:\n"
           "	cncsim/poses_bin - cncsim axes poses in bin format\n"
           "	cncsim/poses - cncsim axes poses\n"
           "\n");
}

int main(int argc, char **argv)
{
    publisher.set_qos(0, 0);
    publisher_txt.set_qos(0, 0);
    control_service.set_qos(2, 50);
    control_service.set_rqos(2, 50);

    const struct option long_options[] = {{"help", no_argument, NULL, 'h'},
                                          {"noprotect", no_argument, NULL, 'p'},
                                          {NULL, 0, NULL, 0}};

    int long_index = 0;
    int opt = 0;

    while ((opt = getopt_long(
                argc, argv, "usvdibtn", long_options, &long_index)) != -1)
    {
        switch (opt)
        {
        case 'h':
            print_help();
            exit(0);

        case 'p':
            ralgo::global_protection = false;

        case 0:
            break;
        }
    }

    interpreter.init_axes(3);
    interpreter.set_scale(ralgo::vector<cnc_float_type>{1, 1, 1});
    planner.set_gears({100, 100, 100});
    interpreter.set_revolver_frequency(heimer::fast_cycle_frequence());
    revolver.set_steppers(steppers_ptrs, 3);

    ralgo::set_logger(&logger);

    crow::create_udpgate();
    crow::start_spin();

    control_service.install_keepalive(2000, true);

    revolver_thread = std::thread(revolver_thread_function);
    planner_thread = std::thread(planner_thread_function);
    telemetry_thread = std::thread(telemetry_thread_function);

    std::this_thread::sleep_for(100ms);

    nos::println("crow themes:");
    nos::println("[service] cncsim/cli - control api");
    nos::println("[topic] cncsim/poses - show poses of axes");
    nos::println("[topic] cncsim/log - runtime information");

    while (1)
    {
        auto exp_str = nos::readline(1024, true);
        if (exp_str.is_ok())
        {
            mtx.lock();
            auto ans = interpreter.newline(*exp_str);
            mtx.unlock();
            nos::println(ans);
        }
        else
        {
            exit(-1);
        }
    }
}
