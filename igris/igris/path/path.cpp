#include <igris/binreader.h>
#include <igris/path/path.h>

#include <cstring>

std::string igris::path::dirname(const std::string &path)
{
    const char *strt = path.c_str();
    const char *ptr = strt;
    const char *last = strt;

    const char *delimeters = "/\\";

    while (*ptr)
    {
        if (strchr(delimeters, *ptr))
            last = ptr;
        ++ptr;
    }

    return std::string(strt, last - strt);
}
