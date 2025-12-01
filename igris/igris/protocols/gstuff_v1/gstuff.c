#include <igris/protocols/gstuff_v1/gstuff.h>
#include <igris/util/crc.h>

int gstuffing_v1(char *data, int size, char *outdata)
{
    char *outstrt;
    uint8_t crc;

    crc = 0xFF;
    outstrt = outdata;

    *outdata++ = GSTUFF_START_V1;

    while (size--)
    {
        char c = *data++;
        igris_strmcrc8(&crc, c);

        switch (c)
        {
        case GSTUFF_START_V1:
            *outdata++ = GSTUFF_STUB_V1;
            *outdata++ = GSTUFF_STUB_START_V1;
            break;

        case GSTUFF_STUB_V1:
            *outdata++ = GSTUFF_STUB_V1;
            *outdata++ = GSTUFF_STUB_STUB_V1;
            break;

        default:
            *outdata++ = c;
        }
    }

    *outdata++ = crc;
    *outdata++ = GSTUFF_START_V1;

    return (int)(outdata - outstrt);
}
