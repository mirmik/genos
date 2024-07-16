#include <igris/util/bug.h>
#include <stdlib.h>

extern "C" void *_sbrk_r(int incr) __attribute__((__used__));
void *_sbrk_r(int incr)
{
    BUG();
}

void dummy_sbrk_caller() __attribute__((__used__));
void dummy_sbrk_caller()
{
    _sbrk_r(0);
}
