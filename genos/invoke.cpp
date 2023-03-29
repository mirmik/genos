#include <cstring>
#include <genos/invoke.h>
#include <genos/schedee_api.h>
#include <igris/time/systime.h>
#include <nos/print.h>

int genos::invoke_program(const char *cmd, int argc, const char **argv)
{
    nos::println("invoke_program");
    nos::println("cmd:", cmd);
    nos::println("argc:", argc);

    for (int i = 0; i < argc; i++)
        nos::println("argv[", i, "]:", argv[i]);

    if (strcmp("testproc", cmd) == 0)
    {
        nos::println("testproc");

        genos::current_schedee_msleep(3000);
        nos::println("I am testproc process");

        return 0;
    }

    nos::println("Not found");
    return -1;
}