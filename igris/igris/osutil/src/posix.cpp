#include <fcntl.h>
#include <signal.h>
#include <unistd.h>

#include <cstring>
#include <string>

#include <igris/osutil/fd.h>
#include <igris/osutil/path.h>
#include <igris/util/string.h>

int igris::osutil::nonblock(int fd, bool en)
{
    if (fd < 0)
        return -1;
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0)
        return -1;
    flags = en ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK);
    return fcntl(fd, F_SETFL, flags) == 0;
}

namespace igris
{
    namespace osutil
    {
        /*void setsig(int fd, int sig) {
           fcntl(fd, F_SETOWN, getpid());
           fcntl(fd, F_SETSIG, sig);
           fcntl(fd,F_SETFL,fcntl(fd,F_GETFL) | O_NONBLOCK | O_ASYNC);
           }*/

        void signal(int sig, void (*handler)(int)) { ::signal(sig, handler); }

        std::string getcwd()
        {
            char cwd[1024];
            if (::getcwd(cwd, sizeof(cwd)) != NULL)
            {
                return std::string(cwd, strlen(cwd));
            }
            else
            {
                perror("getcwd() error");
                return std::string();
            }
        }
    }
}

std::string igris::osutil::expanduser(const std::string &input)
{
    return igris::replace(input, "~", getenv("HOME"));
}
