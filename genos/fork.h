#ifndef GENOS_FORK_H
#define GENOS_FORK_H

#include <cstdint>
#include <igris/compiler.h>
#include <string>
#include <vector>

namespace genos
{
    int clone(int (*fn)(void *), void *arg, void *stack, size_t stack_size);
    int execute(const char *cmd);

    // int command_process_v(const std::vector<std::string> &argv);
    // int execute(const char *cmd);
    // int execute(const std::vector<std::string> &vec);
    int start_command_process_v(const char **argv, size_t stacksize);

    int waitpid(intptr_t pid);
    int waitpid_without_displace(intptr_t pid);
}

#endif