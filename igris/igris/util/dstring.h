#ifndef GXX_UTIL_DSTRING_H
#define GXX_UTIL_DSTRING_H

#include <ctype.h>
#include <igris/compiler.h>
#include <stdlib.h>

__BEGIN_DECLS

int bytes_to_dstring(char *out, const void *data, size_t size);

__END_DECLS

#ifdef __cplusplus

#include <igris/buffer.h>
#include <igris/util/hexascii.h>
#include <string>

namespace igris
{
    static inline std::string dstring(const void *data, size_t size)
    {
        std::string ret;
        char *it = (char *)data;
        char *eit = it + size;

        for (; it != eit; ++it)
        {
            if (isprint(*it))
                ret.push_back(*it);
            else if (*it == '\n')
                ret.append("\\n", 2);
            else if (*it == '\t')
                ret.append("\\t", 2);
            else if (*it == '\\')
                ret.append("\\\\", 2);
            else
            {
                char hi = half2hex((uint8_t)((*it & 0xF0) >> 4));
                char low = half2hex((uint8_t)(*it & 0x0F));
                ret.append("\\x", 2);
                ret.push_back(hi);
                ret.push_back(low);
            }
        }

        return ret;
    }

    static inline std::string dstring(igris::buffer buf)
    {
        return dstring(buf.data(), buf.size());
    }

    static inline std::string dstring(const std::string &buf)
    {
        return dstring(buf.data(), buf.size());
    }
}
#endif

#endif
