#ifndef IGRIS_IOVEC_H
#define IGRIS_IOVEC_H

#if __has_include(<sys/uio.h>)
#include <sys/uio.h>
#else
struct iovec
{
    void *iov_base;
    size_t iov_len;
};
#endif

#endif
