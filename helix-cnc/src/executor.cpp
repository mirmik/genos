#include <board_defs.h>
#include <control.h>
#include <executor.h>
#include <feedback.h>
#include <genos/schedee.h>
#include <genos/shellutil.h>
#include <igris/dprint.h>
#include <main.h>
#include <nos/shell/aggregate_executor.h>
#include <ralgo/robo/quadgen.h>
#include <trigger/scanning_command.h>
#include <version.h>

int api_version(const nos::argv &, nos::ostream &os)
{
    nos::println_to(os, VERSION);
    return 0;
}

int board_name(const nos::argv &, nos::ostream &os)
{
    nos::println_to(os, STRINGIFY2(HELIX_BOARD_NAME));
    return 0;
}

int total_axes(const nos::argv &, nos::ostream &os)
{
    nos::println_to(os, TOTAL_CTRL_AXES);
    return 0;
}

int botfreq(const nos::argv &, nos::ostream &os)
{
    extern int64_t BOTTOM_FREQUENCY_MEASURE_COUNTER;
    nos::println_to(os, "measured: ", BOTTOM_FREQUENCY_MEASURE_COUNTER);
    nos::println_to(os, "calculation: ", fast_cycle_frequence());

    return 0;
}

void user_led_set(uint8_t state);
int led(const nos::argv &args, nos::ostream &os)
{
    if (args.size() < 2)
        return -1;

    if (args[1] == "on")
        user_led_set(1);
    else if (args[1] == "off")
        user_led_set(0);
    else
        return -1;

    return 0;
}

int state(const nos::argv &, nos::ostream &os);

extern robo::quadgen *control_quadgens[TOTAL_CTRL_AXES];
int ctrout(const nos::argv &args, nos::ostream &os)
{
    if (args.size() < 3)
        return -1;

    auto outno = args[1].to_int() - 1;
    auto state = args[2].to_int();

    if (outno < 0 || outno >= TOTAL_CTRL_AXES)
    {
        nos::println_to(os, "wrong axis");
        return -1;
    }

    if (state < 0 || state > 3)
    {
        nos::println_to(os, "wrong state");
        return -1;
    }

    control_quadgens[outno]->set_state(state);

    return 0;
}

int incout(const nos::argv &args, nos::ostream &os)
{
    if (args.size() < 2)
        return -1;

    auto outno = args[1].to_int() - 1;

    if (outno < 0 || outno >= TOTAL_CTRL_AXES)
    {
        nos::println_to(os, "wrong axis");
        return -1;
    }

    control_quadgens[outno]->inc();
    return 0;
}

int decout(const nos::argv &args, nos::ostream &os)
{
    if (args.size() < 2)
        return -1;

    auto outno = args[1].to_int() - 1;

    if (outno < 0 || outno >= TOTAL_CTRL_AXES)
    {
        nos::println_to(os, "wrong axis");
        return -1;
    }

    control_quadgens[outno]->dec();
    return 0;
}

int enable_botom_level_cmd(const nos::argv &, nos::ostream &os)
{
    enable_bottom_level();
    return 0;
}

int disable_botom_level_cmd(const nos::argv &, nos::ostream &os)
{
    disable_bottom_level();
    return 0;
}

int get_poses(const nos::argv &, nos::ostream &os)
{
    double poses[TOTAL_CTRL_AXES];
    for (int i = 0; i < TOTAL_CTRL_AXES; ++i)
    {
        poses[i] = control_quadgens[i]->steps_count();
        nos::println_to(os, poses[i]);
    }
    return 0;
}

int print_encoder_state(const nos::argv &, nos::ostream &os)
{
    auto encoder_state_string = encoder_state_as_string();
    nos::println_to(os, encoder_state_string);
    return 0;
}

int print_feedback_state(const nos::argv &, nos::ostream &os)
{
    auto arr = feedback_position_as_array();
    auto encoder_state_string = nos::format("{}", nos::ilist(arr));
    nos::println_to(os, encoder_state_string);
    return 0;
}

int print_help(const nos::argv &, nos::ostream &os)
{
    ralgo::warn("HELP");
    nos::println_to(os, "Help:");
    system_executor.print_help_to(os);
    return 0;
}

nos::executor maincmd_executor(
    {{"api_version", "Get helix/heimer version", api_version},
     {"help",
      "Print help. Use 'cmd help' to print interpreter's help",
      print_help},
     {"version", "Get helix/heimer version", api_version},
     {"board_name", "", board_name},
     {"total_axes", "", total_axes},
     {"encoder", "", print_encoder_state},
     {"fbpos", "", print_feedback_state},
     {"led", "led on/off", led},
     {"ctrout", "set control out state", ctrout},
     {"incout", "inc output state", incout},
     {"decout", "dec output state", decout},
     {"poses", "get poses", get_poses},
     {"enable_bottom", "enable bottom level", enable_botom_level_cmd},
     {"botfreq", "return measured bottom level frequency", botfreq},
     {"disable_bottom", "disable bottom level", disable_botom_level_cmd}});

int testproc_main(int argc, char **argv)
{
    dprln("testproc_main");
    return 0;
}

nos::aggregate_executor system_executor({
    &maincmd_executor,
    &interpreter.executor,
    &genos::schedee_manager_executor,
#ifndef WITHOUT_TRIGGER
    &trigger_executor,
#endif
});
