#ifndef IGRIS_OSUTIL_TIMEOUTED_READ_H
#define IGRIS_OSUTIL_TIMEOUTED_READ_H

#include <fcntl.h>
#include <stdio.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static inline int timeouted_read(int fd, char *data, size_t size, double time)
{
    fd_set set;
    int rv;
    struct timeval timeout;

    timeout.tv_sec = int(time);
    timeout.tv_usec = (time - int(time)) * 1000000;

    FD_ZERO(&set);
    FD_SET(fd, &set);
    rv = select(fd + 1, &set, NULL, NULL, &timeout);
    if (rv == -1)
        return -1;
    else if (rv == 0)
        return 0;
    else
        return read(fd, data, size);
}

#endif