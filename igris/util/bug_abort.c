#include <igris/util/bug.h>
#include <stdlib.h>

void bug_impl(void)
{
    abort();
    while (1)
        ;
}
