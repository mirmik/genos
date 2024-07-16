#ifndef RALGO_command_H
#define RALGO_command_H

#include <igris/compiler.h>

#include <ralgo/heimer/signal.h>
#include <ralgo/heimer/signal_processor.h>

#include <igris/shell/rshell.h>

namespace heimer
{
    int command(int argc, char **argv, char *output, int maxsize, int *ret);
    int command_exec_safe(const char *str, char *output, int maxsize, int *ret);

    void heimer_reinit();
    void heimer_system_init();

    extern struct rshell_command *commands_table;
}

#endif
