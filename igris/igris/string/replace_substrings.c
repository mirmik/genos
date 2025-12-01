#include <igris/math/defs.h>
#include <igris/util/string.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

void replace_substrings(char *buffer,
                        size_t maxsize,
                        const char *input,
                        size_t inlen,
                        const char *sub,
                        size_t sublen,
                        const char *rep,
                        size_t replen)
{
    const char *strit = input;
    const char *streit = input + inlen;
    char *bufit = buffer;

    if (sublen == 0)
    {
        size_t len = __MIN__(maxsize - 1, inlen);
        memcpy(buffer, input, len);
        buffer[len] = 0;
    }

    char *finded;
    while ((finded = igris_memmem(strit, streit - strit, sub, sublen)) != NULL)
    {
        ptrdiff_t step = finded - strit;

        memcpy(bufit, strit, step);
        bufit += step;
        strit += step;

        memcpy(bufit, rep, replen);
        bufit += replen;
        strit += sublen;
    };

    ptrdiff_t lastlen = streit - strit;
    memcpy(bufit, strit, lastlen);
    *(bufit + lastlen) = 0;
}
