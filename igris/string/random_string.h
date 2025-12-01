#ifndef IGRIS_RANDOM_STRING_H
#define IGRIS_RANDOM_STRING_H

#include <unistd.h>

namespace igris
{
    std::string random_string(int len)
    {
        std::string tmp_s;
        static const char alphanum[] = "0123456789"
                                       "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                       "abcdefghijklmnopqrstuvwxyz";

        srand((unsigned)time(NULL) * getpid());

        tmp_s.reserve(len);

        for (int i = 0; i < len; ++i)
            tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];

        return tmp_s;
    }
}

#endif