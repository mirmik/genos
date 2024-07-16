#include <igris/dprint.h>
#include <ralgo/global_protection.h>
#include <stdio.h>
#include <stdlib.h>

volatile bool ralgo::global_protection = true;

void ralgo::set_global_protection(bool en)
{
    global_protection = en;
}

int ralgo::set_global_protection_command(int argc, char *argv[])
{
    if (argc != 2)
    {
        return 0;
    }

    global_protection = atoi(argv[0]);

    return 0;
}
