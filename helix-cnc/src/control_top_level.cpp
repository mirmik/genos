#include <board_defs.h>
#include <crow/nodes/publisher_node.h>
#include <feedback.h>
#include <genos/autom_schedee.h>
#include <main.h>
#include <quadgen.h>
#include <ralgo/disctime.h>
#include <ralgo/heimer/axisctr.h>
#include <ralgo/robo/stepper_controller.h>
#include <servo.h>

extern bool crow_services_inited;

void helper_thread_func();

genos::autom_schedee serve_thread(
    +[](void *arg, int *state) { control_top_level(); }, nullptr);

genos::autom_schedee helper_thread(
    +[](void *arg, int *state) { helper_thread_func(); }, nullptr);

static disctime_t last_send_time = 0;

extern igris::array_view<robo::stepper_controller *> drvs;
extern igris::array_view<heimer::axis_controller *> numered_axes;

extern robo::quadgen *control_quadgens[TOTAL_CTRL_AXES];

void telemetry_publisher_publish(const int64_t *poses, int axes_count);

void feedback_telemetry_publisher_publish(const int64_t *feedback_poses,
                                          int axes_count);

int mark = 0;
void send_telemetry(
    // const std::array<int64_t, TOTAL_CTRL_AXES> &feedback_position,
    // const std::array<int64_t, TOTAL_CTRL_AXES> &current_control_position
)
{
    if (crow_services_inited == 0)
        return;

    // if (mark == 0)
    //    telemetry_publisher_publish(current_control_position.data(),
    //                                interpreter.get_axes_count());

    int64_t c;
    if (mark == 0)
        telemetry_publisher_publish(&c, 1);

    // else
    //     feedback_telemetry_publisher_publish(feedback_position.data(),
    //                                          interpreter.get_axes_count());

    mark++;
    if (mark == 2)
        mark = 0;
}

bool is_need_to_alarm_stop(
    const std::array<int64_t, TOTAL_CTRL_AXES> &feedback_position,
    const std::array<int64_t, TOTAL_CTRL_AXES> &current_control_position)
{
    if (planner.is_halt())
        return false;

    // bool verify_pos = feedback_guard.verify_position(
    //     igris::span<int64_t>((int64_t *)feedback_position.data(),
    //                          feedback_position.size()),
    //     igris::span<int64_t>((int64_t *)current_control_position.data(),
    //                          current_control_position.size()));

    bool verify_tandem = feedback_guard.verify_tandems(igris::span<int64_t>(
        (int64_t *)feedback_position.data(), feedback_position.size()));

    // verify_pos = true;
    // return false;
    return !verify_tandem; //|| !verify_pos;
}

void control_top_level()
{
    planner.serve();
}

void helper_thread_func()
{
    auto curtime = discrete_time();
    {
        auto feedback_position = feedback_position_as_array();
        auto curtime = discrete_time();

        std::array<int64_t, TOTAL_CTRL_AXES> current_control_position;
        for (size_t i = 0; i < interpreter.get_axes_count(); ++i)
            current_control_position[i] =
                int64_t(revolver.get_steppers()[i]->steps_count());

        bool need_to_alarm_stop =
            is_need_to_alarm_stop(feedback_position, current_control_position);

        if (need_to_alarm_stop)
        {
            planner.alarm_stop();
        }
    }

    if (curtime - last_send_time > 200 && igris::millis() > 2000)
    {
        // auto feedback_position = feedback_position_as_array();

        // std::array<int64_t, TOTAL_CTRL_AXES> current_control_position;
        // for (size_t i = 0; i < interpreter.get_axes_count(); ++i)
        //     current_control_position[i] =
        //         int64_t(revolver.get_steppers()[i]->steps_count());

        // send_telemetry(feedback_position, current_control_position);
        send_telemetry();
        last_send_time = curtime;
    }
}

void init_top_level()
{
    serve_thread.set_mnemo("serve");
    serve_thread.set_priority(1);
    serve_thread.start();
    helper_thread.set_mnemo("helper");
    helper_thread.set_priority(1);
    helper_thread.start();
    nos::println("Top level initialization ... OK");
}
