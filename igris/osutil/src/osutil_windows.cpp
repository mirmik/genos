#include <fcntl.h>
#include <stdio.h>
#include <winsock2.h>

#include <igris/osutil/fd.h>

int igris::osutil::nonblock(int fd, bool en)
{
    unsigned long mode = en;
    return (ioctlsocket(fd, FIONBIO, &mode) == 0) ? true : false;
}

namespace igris
{
    namespace osutil
    {
        /*void setsig(int fd, int sig) {
            fcntl(fd, F_SETOWN, getpid());
            fcntl(fd, F_SETSIG, SIGUSR1);
            fcntl(fd,F_SETFL,fcntl(fd,F_GETFL) | O_NONBLOCK | O_ASYNC);
        }*/

        /*void signal(int sig, void(*handler)(int)) {
            ::signal(sig, handler);
        }*/
    }
}
