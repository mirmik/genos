#include <igris/util/hexascii.h>

void hexascii_encode(const void *indata, int size, void *out)
{
    const uint8_t *data = (uint8_t *)indata;
    const uint8_t *eit = (uint8_t *)indata + size;
    uint8_t *oit = (uint8_t *)out;

    for (const uint8_t *it = data; it != eit; ++it)
    {
        *oit++ = half2hex((*it & 0xF0) >> 4);
        *oit++ = half2hex(*it & 0xF);
    }
}

void hexascii_decode(const void *indata, int size, void *out)
{
    if (size % 2 == 1)
        --size;

    if (size <= 0)
        return;

    const uint8_t *data = (uint8_t *)indata;
    const uint8_t *eit = (uint8_t *)indata + size;
    uint8_t *oit = (uint8_t *)out;

    for (const uint8_t *it = data; it != eit; it += 2)
    {
        *oit++ = hex2byte(*it, *(it + 1));
    }
}
