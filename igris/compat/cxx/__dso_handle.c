#include <igris/dprint.h>

__attribute__((weak)) void __dso_handle()
{
    dprln("__dso_handle");
    while (1)
        ;
}
