#ifndef IGRIS_OSUTIL_SIGNAL_H
#define IGRIS_OSUTIL_SIGNAL_H

namespace igris
{
    namespace osutil
    {
        void setsig(int fd, int sig);
        void signal(int sig, void (*handler)(int));
    }
}

#endif
