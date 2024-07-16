#ifndef NOS_UTIL_FD_H
#define NOS_UTIL_FD_H

#include <stdint.h>
#include <stdlib.h>
#include <string>

namespace nos
{
    namespace osutil
    {
        int nonblock(int64_t fd, bool en);
        int write(int64_t fd, const void *data, size_t size);
        int read(int64_t fd, void *data, size_t size);
        int64_t open(const char *data, int mode);
        int close(int64_t fd);
        int access(const std::string &path);
        int access(const char *path);
        bool is_accessible(const std::string &path);
        bool is_accessible(const char *path);
        FILE *fdopen(int64_t fd, const char *mode);
        int64_t fileno(FILE *f);
        char *strdup(const char *s);
    }
}

#endif
