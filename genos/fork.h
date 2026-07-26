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

    /**
     * Wait for a direct child and return its exit code.
     *
     * A clone-created child remains in zombie state until its parent reaps it.
     * Unknown, unrelated, and already reaped PIDs return -1.
     */
    int waitpid(intptr_t pid);

    /**
     * Reap an already exited child, or arm the current schedee for a later
     * SIGCHLD without displacing its context. Returns 0 while waiting.
     */
    int waitpid_without_displace(intptr_t pid);
}

#endif
