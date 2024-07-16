#ifndef IGRIS_UTIL_ERRNO_H
#define IGRIS_UTIL_ERRNO_H

#include <errno.h>
static inline int SET_ERRNO(int a)
{
    errno = a;
    return -1;
}

#endif
