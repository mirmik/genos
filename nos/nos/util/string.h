#ifndef NOS_STRING_H
#define NOS_STRING_H

#include <cctype>
#include <cstdint>
#include <cstring>
#include <list>
#include <random>
#include <string>
#include <string_view>
//#include <unistd.h>
#include <vector>

namespace nos
{
    std::string random_string(int len, uint64_t seed = 0);

    std::string replace(const std::string &src,
                        const std::string &oldsub,
                        const std::string &newsub);

    std::vector<std::string> split(const std::string_view &str,
                                   char delim = ' ');
    std::vector<std::string> split(const std::string_view &str,
                                   const char *delims);

    std::string join(const std::vector<std::string> &, char delim);

    template <class Iter>
    std::string join(Iter start,
                     Iter end,
                     const char *delim,
                     const char *prefix,
                     const char *postfix)
    {
        std::string ret;

        size_t sz = 0;
        size_t tot = 0;

        for (Iter it = start; it != end; ++it)
        {
            sz += it->size();
            ++tot;
        }

        ret.reserve(sz + strlen(prefix) + strlen(postfix) +
                    tot * strlen(delim));

        ret.append(prefix);
        Iter it = start;
        for (unsigned int i = 0; i < tot - 1; ++i)
        {
            ret.append(*it++);
            ret.append(delim);
        }
        ret.append(*it);
        ret.append(postfix);

        return ret;
    }

    std::string trim(const std::string_view &view);
}

#endif
