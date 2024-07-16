#include <errno.h>
#include <igris/math.h>
#include <igris/shell/rshell.h>
#include <string.h>

#include <igris/dprint.h>

#define SSHELL_ARGCMAX 10

int rshell_execute_v(int argc, char **argv,
                     const struct rshell_command *cmdtable, int *retptr,
                     int dropargs, char *output, int maxsize)
{
    int res;
    const struct rshell_command *it = cmdtable;
    while (it->func != NULL)
    {
        if (!strcmp(argv[0], it->name))
        {
            res = it->func(argc - dropargs, argv + dropargs, output, maxsize);
            if (retptr)
                *retptr = res;
            return SSHELL_OK;
        }
        ++it;
    }

    return ENOENT;
}

int rshell_execute(char *str, const struct rshell_command *cmdtable,
                   int *retptr, int dropargs, char *output, int maxsize)
{
    char *argv[SSHELL_ARGCMAX];
    int argc;

    if (*str == '\0')
    {
        return 0;
    }

    argc = argvc_internal_split(str, argv, SSHELL_ARGCMAX);

    return rshell_execute_v(argc, argv, cmdtable, retptr, dropargs, output,
                            maxsize);
}

int rshell_tables_execute(char *str, const struct rshell_command_table *tables,
                          int *retptr, char *output, int maxsize)
{
    char *argv[SSHELL_ARGCMAX];
    int argc;
    int res;

    if (*str == '\0')
    {
        return 0;
    }

    argc = argvc_internal_split(str, argv, SSHELL_ARGCMAX);

    if (argc == 0)
        return 0;

    const struct rshell_command_table *tit = tables;
    while (tit->table != NULL)
    {
        const struct rshell_command *it = tit->table;
        while (it->func != NULL)
        {
            if (!strcmp(argv[0], it->name))
            {
                res = it->func(argc - tit->dropargs, argv + tit->dropargs,
                               output, maxsize);
                if (retptr)
                    *retptr = res;
                return SSHELL_OK;
            }
            ++it;
        }
        ++tit;
    }

    return ENOENT;
}

int rshell_help(const struct rshell_command *cmdtable, char *ans, int ansmax)
{
    int len = 0;
    int l;

    const struct rshell_command *it = cmdtable;
    while (it->func != NULL)
    {
        memcpy(ans + len, it->name,
               l = __MIN__((int)strlen(it->name), ansmax - len - 1));
        len += l;

        if (it->help)
        {
            memcpy(ans + len, " - ", l = __MIN__(3, ansmax - len - 1));
            len += l;

            memcpy(ans + len, it->help,
                   l = __MIN__((int)strlen(it->help), ansmax - len - 1));
            len += l;
        }
        memcpy(ans + len, "\r\n", l = __MIN__(2, ansmax - len - 1));
        len += l;
        ++it;
    }

    ans[len] = '\0';
    return len;
}

int rshell_tables_help(const struct rshell_command_table *cmdtables, char *ans,
                       int ansmax)
{
    int len = 0;

    const struct rshell_command_table *tit = cmdtables;
    while (tit->table != NULL)
    {
        len += rshell_help(tit->table, ans + len, ansmax - len - 1);
        ++tit;
    }

    ans[len] = '\0';
    return len;
}
