#include <errno.h>
#include <igris/shell/mshell.h>
#include <stddef.h>
#include <string.h>

#define SSHELL_ARGCMAX 10

int mshell_execute(char *str,
                   const struct mshell_command *cmdtable,
                   int *retptr)
{
    char *argv[SSHELL_ARGCMAX];
    int argc;
    int res;

    if (*str == '\0')
    {
        return ENOENT;
    }

    argc = argvc_internal_split(str, argv, SSHELL_ARGCMAX);

    const struct mshell_command *it = cmdtable;
    while (it->func != NULL)
    {
        if (!strcmp(argv[0], it->name))
        {
            res = it->func(argc, argv);
            if (retptr)
                *retptr = res;
            return SSHELL_OK;
        }
        ++it;
    }

    return ENOENT;
}

int mshell_tables_execute(char *str,
                          const struct mshell_command *const *tables,
                          int *retptr)
{
    char *argv[SSHELL_ARGCMAX];
    int argc;
    int res;

    if (*str == '\0')
    {
        return ENOENT;
    }

    argc = argvc_internal_split(str, argv, SSHELL_ARGCMAX);

    const struct mshell_command *const *tit = tables;
    while (*tit != NULL)
    {
        const struct mshell_command *it = *tit;
        while (it->func != NULL)
        {
            if (!strcmp(argv[0], it->name))
            {
                res = it->func(argc, argv);
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

void mshell_help(const struct mshell_command *cmdtable,
                 void (*write)(void *, const char *, size_t),
                 void *privdata)
{
    const struct mshell_command *it = cmdtable;
    while (it->func != NULL)
    {
        write(privdata, it->name, strlen(it->name));
        if (it->help)
        {
            write(privdata, " - ", 3);
            write(privdata, it->help, strlen(it->help));
        }
        write(privdata, "\r\n", 2);
        ++it;
    }
}

void mshell_tables_help(const struct mshell_command *const *cmdtables,
                        void (*write)(void *, const char *, size_t),
                        void *privdata)
{
    const struct mshell_command *const *tit = cmdtables;
    while (*tit != NULL)
    {
        mshell_help(*tit, write, privdata);
        ++tit;
    }
}
