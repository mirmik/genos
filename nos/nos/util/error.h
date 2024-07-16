#ifndef NOS_ERROR_H
#define NOS_ERROR_H

#include <string>

namespace nos
{
    void fatal_handle(const char *msg);

    class errstring : public std::string
    {
    public:
        errstring(const char *msg) : std::string(msg) {}
        errstring(const std::string &msg) : std::string(msg) {}
        ~errstring() = default;
    };
}

#endif
