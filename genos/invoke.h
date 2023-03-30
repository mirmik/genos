#ifndef GENOS_INVOKE_H
#define GENOS_INVOKE_H

#define GENOS_DEFAULT_HEAPSTACK_SIZE 1024

namespace genos
{
    int invoke_program(const char *cmd, int argc, const char **argv);

    int system(const char *cmd);
    int system_v(const char **cmds);
    int system_v_without_displace(const char **cmds);
}

#endif