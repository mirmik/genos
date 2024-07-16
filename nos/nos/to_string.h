#ifndef NOS_TO_STRING_H
#define NOS_TO_STRING_H

#include <nos/io/sstream.h>
#include <nos/print.h>

namespace nos
{
    template <class T> std::string to_string(const T &t)
    {
        nos::stringstream oss;
        nos::print_to(oss, t);
        return oss.str();
    }

    template <class V>
    std::string to_string_join(const V &vec, const std::string &sep)
    {
        std::string res;
        for (auto &v : vec)
        {
            res += nos::to_string(v) + sep;
        }

        size_t sep_size = sep.size();
        if (res.size() >= sep_size)
        {
            res.resize(res.size() - sep_size);
        }

        return res;
    }
}

#endif