#include <ralgo/heimer/command.h>

#include <cstdlib>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <igris/datastruct/argvc.h>
#include <igris/dprint.h>
#include <igris/shell/rshell.h>

#include <ralgo/heimer/axis_stub_processor.h>
#include <ralgo/heimer/axisapi.h>
#include <ralgo/heimer/axisctr.h>
#include <ralgo/heimer/axstate_linear_processor.h>
#include <ralgo/heimer/axstate_pid_processor.h>
#include <ralgo/heimer/axstate_pose3_chain_processor.h>
#include <ralgo/heimer/axstate_sincos_processor.h>
#include <ralgo/heimer/dof6_controller.h>
#include <ralgo/heimer/dof6_signal.h>
#include <ralgo/heimer/executor.h>
#include <ralgo/heimer/phase_mux_processor.h>
#include <ralgo/heimer/phase_signals.h>
#include <ralgo/heimer/scalar_signal.h>
#include <ralgo/heimer/sigtypes.h>
#include <ralgo/heimer/velocity_applier.h>

#include <ralgo/robo/stepper_controller.h>

using namespace heimer;

static int help(int, char **, char *output, int maxsize)
{
    snprintf(output,
             maxsize,
             "Command list:\r\n"
             "signal - signal api utility\r\n");

    return 0;
}

int heimer::command_exec_safe(const char *str,
                              char *output,
                              int maxsize,
                              int *ret)
{
    char copydata[96];
    char *argv[10];
    int argc;

    strncpy(copydata, str, 96);

    argc = argvc_internal_split(copydata, argv, 10);
    return command(argc, argv, output, maxsize, ret);
}

void heimer::heimer_system_init()
{
    signal_head_list_reinit();
    signal_processors_list_reinit();
}

static int ctrnew(int argc, char **argv, char *output, int maxsize)
{
    if (argc < 2)
    {
        snprintf(output, maxsize, "usage: ctrnew TYPE NAME\r\n");
        return -1;
    }

    if (strcmp(argv[0], "axisctr") == 0)
    {
        if (argc < 2)
        {
            snprintf(output,
                     maxsize,
                     "usage: ctrnew axisctr NAME [NAME2] [NAME3] [...]\r\n");
            return -1;
        }

        for (int i = 1; i < argc; ++i)
        {
            create_axis_controller(argv[i]);
        }
        return 0;
    }

    if (strcmp(argv[0], "axlinear") == 0)
    {
        if (argc < 3)
        {
            snprintf(output, maxsize, "usage: ctrnew axlinear NAME DIM\r\n");
            return -1;
        }

        int dim = strtol(argv[2], NULL, 10);
        auto ptr = new heimer::axstate_linear_processor(
            argv[1], dim, nullptr, nullptr, nullptr, nullptr);
        ptr->allocate_resources();
        return 0;
    }

    if (strcmp(argv[0], "axsincos") == 0)
    {
        if (argc < 2)
        {
            snprintf(output, maxsize, "usage: ctrnew axsincos NAME\r\n");
            return -1;
        }

        new heimer::axstate_sincos_processor(argv[1]);
        return 0;
    }

    if (strcmp(argv[0], "axstub") == 0)
    {
        if (argc < 2)
        {
            snprintf(output, maxsize, "usage: ctrnew axstub NAME\r\n");
            return -1;
        }

        for (int i = 1; i < argc; ++i)
        {
            new heimer::axis_stub_processor(argv[i]);
        }
        return 0;
    }

    if (strcmp(argv[0], "axpid") == 0)
    {
        if (argc < 2)
        {
            snprintf(output, maxsize, "usage: ctrnew axpid NAME\r\n");
            return -1;
        }

        new heimer::axstate_pid_processor(argv[1]);
        return 0;
    }

    if (strcmp(argv[0], "chain3linctr") == 0)
    {
        if (argc < 3)
        {
            snprintf(output, maxsize, "usage: ctrnew chain3ctr DIM NAME\r\n");
            return -1;
        }

        int dim = strtol(argv[1], NULL, 10);
        auto ptr =
            new heimer::axstate_chain3_translation_processor(argv[2], dim);
        ptr->allocate_resources();
        return 0;
    }

    if (strcmp(argv[0], "dof6ctr") == 0)
    {
        if (argc < 2)
        {
            snprintf(output, maxsize, "usage: ctrnew dof6ctr NAME\r\n");
            return -1;
        }

        new heimer::dof6_controller(argv[1]);
        return 0;
    }

    if (strcmp(argv[0], "axmuxctr") == 0)
    {
        if (argc < 3)
        {
            snprintf(output, maxsize, "usage: ctrnew axmuxctr DIM NAME\r\n");
            return -1;
        }

        int dim = atoi(argv[1]);
        const char *name = argv[2];

        if (dim == 2)
            new phase_mux_processor<2>(name);

        if (dim == 3)
            new phase_mux_processor<3>(name);

        return 0;
    }

    snprintf(output,
             maxsize,
             "Unresolved TYPE. Possible types: axisctr, axlinear, "
             "axsincos, axstub, chain3linctr, axmuxctr\r\n");
    return -1;
}

static int ctr(int argc, char **argv, char *output, int maxsize)
{
    char *name = argv[0];
    signal_processor *ctr = signal_processor_get_by_name(name);

    if (ctr == NULL)
    {
        snprintf(output, maxsize, "Processor not found.\r\n");
        return ENOENT;
    }

    return ctr->command(argc - 1, argv + 1, output, maxsize);
}

static int signew(int argc, char **argv, char *output, int maxsize)
{
    if (argc < 2)
    {
        snprintf(output, maxsize, "usage: signew TYPE NAME\r\n");
        return -1;
    }

    if (strcmp(argv[0], "axstate") == 0)
    {
        for (int i = 1; i < argc; ++i)
        {
            const char *name = argv[i];
            new axis_state(
                name); // Сохранение указателя происходит внутри конструктора.
        }
        return 0;
    }

    if (strcmp(argv[0], "scalar") == 0)
    {
        for (int i = 1; i < argc; ++i)
        {
            const char *name = argv[i];
            new scalar_signal(name);
        }
        return 0;
    }

    if (strcmp(argv[0], "dof6state") == 0)
    {
        for (int i = 1; i < argc; ++i)
        {
            const char *name = argv[i];
            new dof6_signal(name);
        }
        return 0;
    }

    if (strcmp(argv[0], "axphase") == 0)
    {
        int dim = atoi(argv[1]);

        for (int i = 2; i < argc; ++i)
        {
            const char *name = argv[i];

            if (dim == 2)
                new phase_signal<2>(name);

            if (dim == 3)
                new phase_signal<3>(name);
        }
        return 0;
    }

    snprintf(output,
             maxsize,
             "Unresolved TYPE. Possible types: axstate, scalar, dof6state, "
             "axphase\r\n");
    return -1;
}

static int sig(int argc, char **argv, char *output, int maxsize)
{
    char *signame = argv[0];
    signal_head *sig = signal_get_by_name(signame);

    if (sig == NULL)
    {
        snprintf(output, maxsize, "Signal not found.");
        return ENOENT;
    }

    return sig->command_v(argc - 1, argv + 1, output, maxsize);
}

static int ctrlist(int, char **, char *output, int maxsize)
{
    char buf[48];

    signal_processor *it;
    dlist_for_each_entry(it, &signal_processor_list, list_lnk)
    {
        memset(buf, 0, 48);
        snprintf(buf,
                 48,
                 "%*s is_active:%d\r\n",
                 SIGNAL_PROCESSOR_NAME_MAX_LENGTH,
                 it->name().data(),
                 it->is_active());
        strncat(output, buf, maxsize);
    }

    return 0;
}

static int siglist(int, char **, char *output, int maxsize)
{
    char buf[48];

    signal_head *it;
    dlist_for_each_entry(it, &signals_list, list_lnk)
    {
        memset(buf, 0, 48);
        snprintf(buf,
                 48,
                 "%*s controlled: %d\r\n",
                 SIGNAL_PROCESSOR_NAME_MAX_LENGTH,
                 it->name.c_str(),
                 it->current_controller != nullptr);
        strncat(output, buf, maxsize);
    }

    return 0;
}

static int stepsim(int argc, char **argv, char *, int)
{
    for (int i = 0; i < argc; ++i)
    {
        auto *stpr = new robo::stepper;
        auto *sctr = new robo::fixed_frequency_stepper_controller(stpr);
        auto *vctr = new heimer::velocity_applier(argv[i], sctr);
        (void)vctr;

        sctr->bind();
        sctr->set_frequency(fast_cycle_frequence());
    }

    return 0;
}

static int execcmd(int argc, char **argv, char *output, int maxsize)
{
    return executor_command(argc, argv, output, maxsize);
}

static int confcmd(int, char **argv, char *, int)
{
    if (strcmp(argv[0], "debugacts") == 0)
    {
        heimer::debug_activations = atoi(argv[1]);
    }

    if (strcmp(argv[0], "protect") == 0)
    {
        ralgo::global_protection = atoi(argv[1]);
    }

    if (strcmp(argv[0], "initaxapi") == 0)
    {
        collect_axis_api();
    }

    return 0;
}

static struct rshell_command commands[] = {{"help", help, NULL},
                                           {"sig", sig, NULL},
                                           {"ctr", ctr, NULL},
                                           {"ctrnew", ctrnew, NULL},
                                           {"signew", signew, NULL},
                                           {"ctrlist", ctrlist, NULL},
                                           {"siglist", siglist, NULL},
                                           {"stepsim", stepsim, NULL},
                                           {"config", confcmd, NULL},
                                           {"exec", execcmd, NULL},
                                           {"ax", axises_api_command, NULL},
                                           {"ig", igroups_api_command, NULL},
                                           {NULL, NULL, NULL}};
struct rshell_command *heimer::commands_table = commands;

int heimer::command(int argc, char **argv, char *output, int maxsize, int *ret)
{
    int sts = rshell_execute_v(argc,
                               argv,
                               commands,
                               ret,
                               1, // drop submenu name
                               output,
                               maxsize);

    return sts;
}

void heimer::heimer_reinit()
{
    signal_head_list_reinit();
    signal_processors_list_reinit();
}
