#ifndef IGRIS_STRING_H
#define IGRIS_STRING_H

#include <igris/compiler.h>
#include <stdlib.h>

__BEGIN_DECLS

void *igris_memmem(const void *l, size_t l_len, const void *s, size_t s_len);

void replace_substrings(char *buffer,
                        size_t maxsize,
                        const char *input,
                        size_t inlen,
                        const char *sub,
                        size_t sublen,
                        const char *rep,
                        size_t replen);

__END_DECLS

#ifdef __cplusplus

#include <cctype>
#include <cstdint>
#include <cstring>
#include <igris/util/hexascii.h>
#include <list>
#include <string>
#include <vector>

#include <igris/buffer.h>

namespace igris
{
    std::string replace(const std::string &src,
                        const std::string &oldsub,
                        const std::string &newsub);

    std::vector<std::string> split(const igris::buffer &str, char delim = ' ');
    std::vector<std::string> split(const igris::buffer &str,
                                   const char *delims);
    std::vector<std::string> split_cmdargs(const igris::buffer &str);

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

    static inline std::string trim(const igris::buffer &view)
    {
        if (view.size() == 0)
            return "";

        // size_t strt = view.find_first_not_of(" \n\t");
        // size_t fini = view.find_last_not_of(" \n\t") + 1;

        const char *left = view.data();
        const char *right = view.data() + view.size() - 1;
        const char *end = view.data() + view.size();

        while (left != end && (*left == ' ' || *left == '\n' || *left == '\r' ||
                               *left == '\t'))
            ++left;

        if (left == end)
            return "";

        while (left != right && (*right == ' ' || *right == '\n' ||
                                 *right == '\r' || *right == '\t'))
            --right;

        return std::string(left, (right - left) + 1);
    }

    /*static inline igris::buffer trim_left(const igris::buffer &view)
    {
        if (view.size() == 0)
            return view;

        size_t fini = view.find_last_not_of(" \n\t") + 1;
        return view.substr(0, fini);
    }

    static inline igris::buffer trim_right(const igris::buffer &view)
    {
        if (view.size() == 0)
            return view;

        size_t strt = view.find_first_not_of(" \n\t");
        return view.substr(strt, view.size() - strt);
    }*/
}

#endif

#endif
