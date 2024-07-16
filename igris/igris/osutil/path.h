#ifndef IGRIS_OSUTIL_PATH_H
#define IGRIS_OSUTIL_PATH_H

#include <string>

namespace igris
{
    namespace osutil
    {
        std::string expanduser(const std::string &input);
    }
}

#endif
