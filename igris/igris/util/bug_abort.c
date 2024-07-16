#include <igris/util/bug.h>
#include <stdlib.h>

void bug_impl()
{
    abort();
    while (1)
        ;
}
