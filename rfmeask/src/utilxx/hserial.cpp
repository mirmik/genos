#include "utilxx/hserial.h"
#include "string.h"
#include "utilxx/ascii_convert.h"
#include <igris/util/bits.h>

unsigned int __hserial_half(char *buf, uint8_t *ref, uint8_t flag)
{
    if (bits_mask_eq(flag, SERDIR_MASK, SER))
        *buf = hbyte2hex(*ref & 0xF);
    else
        *ref = hex2hbyte(*buf);
    return 1;
}

unsigned int __hserial(char *buf, void *ref, uint16_t len, uint8_t flag)
{
    bool reverse = is_need_reverse(bits_mask(flag, PROTOORD_MASK));
    if (bits_mask_eq(flag, SERDIR_MASK, SER))
    {
        if (reverse)
            bytes2hex_swap(buf, ref, len);
        else
            bytes2hex(buf, ref, len);
    }
    else
    {
        if (reverse)
            hex2bytes_swap(ref, buf, len);
        else
            hex2bytes(ref, buf, len);
    }
    return len * 2;
}
