/**
    Полный аналог mshell, с единственным отличием - используется другой тип
   указателя.
*/

#ifndef IGRIS_SHELL_RSHELL_H
#define IGRIS_SHELL_RSHELL_H

#include <igris/compiler.h>
#include <igris/datastruct/argvc.h>

#define SSHELL_OK 0

//При отсутствии ноды возвращает ENOENT

struct rshell_command
{
    const char *name;
    int (*func)(int, char **, char *, int);
    const char *help;
};

struct rshell_command_table
{
    const struct rshell_command *table;
    int dropargs;
};

__BEGIN_DECLS

int rshell_execute_v(int argc, char **argv, const struct rshell_command *cmd,
                     int *retptr, int dropargs, char *output, int maxsize);
int rshell_execute(char *str, const struct rshell_command *cmd, int *retptr,
                   int dropargs, char *output, int maxsize);

int rshell_tables_execute(char *str,
                          const struct rshell_command_table *cmdtable,
                          int *retptr, char *output, int maxsize);

int rshell_help(const struct rshell_command *cmd, char *ans, int ansmax);

int rshell_tables_help(const struct rshell_command_table *cmd, char *ans,
                       int ansmax);

__END_DECLS

#endif
