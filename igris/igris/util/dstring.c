#include <igris/util/ctype.h>
#include <igris/util/dstring.h>
#include <igris/util/hexascii.h>

int bytes_to_dstring(char *out, const void *data, size_t size)
{
    char *it = (char *)data;
    char *eit = it + size;
    char *dst = out;

    for (; it != eit; ++it)
    {
        char c = *it;
        if (igris_isprint(c))
            *dst++ = *it;

        else if (*it == '\n')
        {
            *dst++ = '\\';
            *dst++ = 'n';
        }

        else if (*it == '\t')
        {
            *dst++ = '\\';
            *dst++ = 't';
        }

        else if (*it == '\\')
        {
            *dst++ = '\\';
            *dst++ = '\\';
        }

        else
        {
            uint8_t hi = half2hex((*it & 0xF0) >> 4);
            uint8_t low = half2hex(*it & 0x0F);
            *dst++ = '\\';
            *dst++ = 'x';
            *dst++ = hi;
            *dst++ = low;
        }
    }
    *dst = '\0';

    return (int)(dst - out);
}
