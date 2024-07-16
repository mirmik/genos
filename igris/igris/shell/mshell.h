#ifndef IGRIS_SIMPLE_SHELL_H
#define IGRIS_SIMPLE_SHELL_H

#include <igris/compiler.h>
#include <igris/datastruct/argvc.h>

#define SSHELL_OK 0

//При отсутствии ноды возвращает ENOENT

struct mshell_command
{
    const char *name;
    int (*func)(int, char **);
    const char *help;
};

__BEGIN_DECLS

int mshell_execute(char *str, const struct mshell_command *cmd, int *retptr);

int mshell_tables_execute(char *str,
                          const struct mshell_command *const *cmd,
                          int *retptr);

void mshell_help(const struct mshell_command *cmd,
                 void (*write)(void *, const char *, size_t),
                 void *privdata);

void mshell_tables_help(const struct mshell_command *const *cmd,
                        void (*write)(void *, const char *, size_t),
                        void *privdata);

__END_DECLS

#endif
