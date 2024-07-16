#include <ctype.h>
#include <functions.h>
#include <genos/schedee.h>
#include <igris/dprint.h>
#include <igris/util/numconvert.h>
#include <utility/mshell.h>

#include "main.h"

int numcmd_info(int argc, char **argv)
{
    //    numcmd_debug_info(&numcmd_cntxt);
    return 0;
}

int doevent(int argc, char **argv)
{
    if (argc < 2 || !isdigit(*argv[1]))
    {
        serial2_uartring.println("Second arg should be integer");
        return -1;
    }

    eventctr.do_event(igris_atou32(argv[1], 10, NULL));
    return 0;
}

int hello(int argc, char **argv)
{
    serial2_uartring.println("HelloWorld");
    return 0;
}

extern int pulse_loop_count;
extern genos::ktimer pulse_timer;
int pulsespam(int argc, char **argv)
{
    if (argc != 3)
    {
        serial2_uartring.println(
            "Usage: pulsespam PERIOD_MS TOTAL\n@PERIOD_MS == 0 - disable");
    }

    int arg = igris_atoi32(argv[1], 10, nullptr);
    int total = argc == 3 ? igris_atoi32(argv[2], 10, nullptr) : -1;

    system_lock();
    if (arg == 0)
    {
        pulse_timer.unplan();
    }

    else
    {
        pulse_loop_count = total;
        pulse_timer.set_start_now();
        pulse_timer.set_interval_ms(arg);
        pulse_timer.plan();
    }
    system_unlock();

    return 0;
}

bool trigin_info_mode = false;
int trigin_info(int argc, char **argv)
{
    if (argc != 2)
        serial2_uartring.println("usage: trigin_info [0|1]");
    trigin_info_mode = atoi(argv[1]);
    return 0;
}

bool eventctr_info_mode = false;
int eventctr_info(int argc, char **argv)
{
    if (argc != 2)
        serial2_uartring.println("usage: eventctr_info [0|1]");
    eventctr_info_mode = atoi(argv[1]);
    return 0;
}

int encoder(int argc, char **argv)
{
    DPRINT(axis_x.read_encoder());
    DPRINT(axis_y.read_encoder());
    return 0;
}

static int discout(int argc, char **argv)
{
    if (argc != 3)
    {
        serial2_uartring.println("usage: out NUM STATE");
        return -1;
    }

    int num = igris_atoi32(argv[1], 10, NULL);
    int state = igris_atoi32(argv[2], 10, NULL);

    if (num >= 4)
    {
        serial2_uartring.println("NUM must be included in interval [0..3]");
        return -1;
    }

    set_discrete_output_state(num, state);

    return 0;
}

static int unicout(int argc, char **argv)
{
    if (argc != 2)
    {
        serial2_uartring.println("usage: unicout NUM");
        return -1;
    }

    int num = igris_atoi32(argv[1], 10, NULL);

    if (num >= 4)
    {
        serial2_uartring.println("NUM must be included in interval [0..3]");
        return -1;
    }

    set_discrete_output_unical(num);

    return 0;
}

static int disouts(int argc, char **argv)
{
    disable_all_discrete_outputs();
    return 0;
}

static int startx(int argc, char **argv)
{
    set_scan_cmpctr_x();
    return 0;
}

int testproc_func(void *arg)
{
    dprln("testproc_func");
    return 0;
}

static int testproc(int argc, char **argv)
{
    genos::create_process(testproc_func, 0, 0, 1024);
    genos::kill(124);
    return 0;
}

struct mshell_command mshell_commands_table[] = {
    {"hello", hello, MSHELL_FUNCTION, "test function invoke"},

    {"doevent",
     doevent,
     MSHELL_FUNCTION,
     "eventctr event number do. Usage: doevent EVNO"},
    {"eventlist", print_eventlist, MSHELL_FUNCTION, "print eventctr list"},

    {"nac",
     (int (*)(int, char **))trigout_send_pulse,
     MSHELL_FUNCTION,
     "generate manual trigger"},
    {"stopctr",
     (int (*)(int, char **))stop_x,
     MSHELL_FUNCTION,
     "stop controller"},

    {"out", discout, MSHELL_FUNCTION, "manual set discrete output state"},
    {"unicout", unicout, MSHELL_FUNCTION, "manual set unical discrete output"},
    {"disouts", disouts, MSHELL_FUNCTION, "disable all discrete outputs"},

    {"encoder",
     (int (*)(int, char **))encoder,
     MSHELL_FUNCTION,
     "read encoders"},
    {"trigin_info",
     (int (*)(int, char **))trigin_info,
     MSHELL_FUNCTION,
     "enable/disable trigin debug information"},
    {"eventctr_info",
     (int (*)(int, char **))eventctr_info,
     MSHELL_FUNCTION,
     "enable/disable eventctr debug information"},

    {"pulsespam",
     (int (*)(int, char **))pulsespam,
     MSHELL_FUNCTION,
     "enable/disable pulsespam"},

    {"startx",
     (int (*)(int, char **))startx,
     MSHELL_FUNCTION,
     "start x controller"},

    {"testproc",
     (int (*)(int, char **))testproc,
     MSHELL_FUNCTION,
     "start testproc"},

    MSHELL_TBLFIN};