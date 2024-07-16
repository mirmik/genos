#ifndef TRENT_PARSE_HELPER_H
#define TRENT_PARSE_HELPER_H

#include <nos/trent/trent.h>

static inline std::vector<std::string>
trent_get_string_vector(const nos::trent &dict)
{
    std::vector<std::string> ret;
    const auto &source = dict.as_list();
    std::transform(source.begin(),
                   source.end(),
                   std::back_inserter(ret),
                   [&](const auto &arg) { return arg.as_string(); });
    return ret;
}

#endif