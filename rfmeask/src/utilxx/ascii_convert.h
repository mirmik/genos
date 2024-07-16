/**
    @file
*/

#ifndef ASCII_CONVERT_H
#define ASCII_CONVERT_H

#include "inttypes.h"
#include "utilxx/charbuf.h"

union uint8_accessor
{
    uint8_t num;
    struct
    {
        uint8_t l : 4;
        uint8_t h : 4;
    } half;
    struct
    {
        uint8_t bit0 : 1;
        uint8_t bit1 : 1;
        uint8_t bit2 : 1;
        uint8_t bit3 : 1;
        uint8_t bit4 : 1;
        uint8_t bit5 : 1;
        uint8_t bit6 : 1;
        uint8_t bit7 : 1;
    } bits;
};

union uint32_accessor
{
    uint32_t num;
    struct
    {
        uint8_t llo;
        uint8_t lhi;
        uint8_t hlo;
        uint8_t hhi;
    } byte;
    struct
    {
        uint8_t llo_l : 4;
        uint8_t llo_h : 4;
        uint8_t lhi_l : 4;
        uint8_t lhi_h : 4;
        uint8_t hlo_l : 4;
        uint8_t hlo_h : 4;
        uint8_t hhi_l : 4;
        uint8_t hhi_h : 4;
    } half;
};

union uint64_accessor
{
    uint64_t num;
    struct
    {
        uint8_t lllo;
        uint8_t llhi;
        uint8_t lhlo;
        uint8_t lhhi;
        uint8_t hllo;
        uint8_t hlhi;
        uint8_t hhlo;
        uint8_t hhhi;
    } byte;
    struct
    {
        uint8_t lllo_l : 4;
        uint8_t lllo_h : 4;
        uint8_t llhi_l : 4;
        uint8_t llhi_h : 4;
        uint8_t lhlo_l : 4;
        uint8_t lhlo_h : 4;
        uint8_t lhhi_l : 4;
        uint8_t lhhi_h : 4;
        uint8_t hllo_l : 4;
        uint8_t hllo_h : 4;
        uint8_t hlhi_l : 4;
        uint8_t hlhi_h : 4;
        uint8_t hhlo_l : 4;
        uint8_t hhlo_h : 4;
        uint8_t hhhi_l : 4;
        uint8_t hhhi_h : 4;
    } half;
};

union uint16_accessor
{
    uint16_t num;
    struct
    {
        uint8_t lo;
        uint8_t hi;
    } byte;
    struct
    {
        uint8_t lo_l : 4;
        uint8_t lo_h : 4;
        uint8_t hi_l : 4;
        uint8_t hi_h : 4;
    } half;
};

static inline uint8_t hexascii_to_halfbyte(char c)
{
    return (uint8_t)(c <= '9' ? c - '0' : c - 'A' + 10);
}
//#define hex2hbyte(a) hexascii_to_halfbyte(a)

static inline uint8_t hexascii2_to_byte(char hi, char lo)
{
    uint8_accessor ret;
    ret.half.h = hexascii_to_halfbyte(hi);
    ret.half.l = hexascii_to_halfbyte(lo);
    return ret.num;
}

static inline char halfbyte_to_hexascii(uint8_t n)
{
    return n < 10 ? '0' + n : 'A' - 10 + n;
}
//#define hbyte2hex(a) halfbyte_to_hexascii(a)

#define get_halfbyte_number(u, n) (u >> (4 * (n)) & 0xF)

static inline charbuf<2> uint8_to_hexcode(uint8_t src)
{
    uint8_accessor acs;
    acs.num = src;
    charbuf<2> buf;
    buf[1] = halfbyte_to_hexascii(acs.half.l);
    buf[0] = halfbyte_to_hexascii(acs.half.h);
    return buf;
}

static inline charbuf<4> uint16_to_hexcode(uint16_t src)
{
    uint16_accessor acs;
    acs.num = src;
    charbuf<4> buf;
    buf[3] = halfbyte_to_hexascii(acs.half.lo_l);
    buf[2] = halfbyte_to_hexascii(acs.half.lo_h);
    buf[1] = halfbyte_to_hexascii(acs.half.hi_l);
    buf[0] = halfbyte_to_hexascii(acs.half.hi_h);
    return buf;
}

static inline charbuf<8> uint32_to_hexcode(uint32_t src)
{
    uint32_accessor acs;
    acs.num = src;
    charbuf<8> buf;
    buf[7] = halfbyte_to_hexascii(acs.half.llo_l);
    buf[6] = halfbyte_to_hexascii(acs.half.llo_h);
    buf[5] = halfbyte_to_hexascii(acs.half.lhi_l);
    buf[4] = halfbyte_to_hexascii(acs.half.lhi_h);
    buf[3] = halfbyte_to_hexascii(acs.half.hlo_l);
    buf[2] = halfbyte_to_hexascii(acs.half.hlo_h);
    buf[1] = halfbyte_to_hexascii(acs.half.hhi_l);
    buf[0] = halfbyte_to_hexascii(acs.half.hhi_h);
    return buf;
}

static inline uint8_t hexcode_to_uint8(const char *str)
{
    uint8_t ret = hexascii2_to_byte(*str, *(str + 1));
    return ret;
}

static inline uint16_t hexcode_to_uint16(const char *str)
{
    uint16_accessor s;
    s.byte.hi = hexascii2_to_byte(*(str + 0), *(str + 1));
    s.byte.lo = hexascii2_to_byte(*(str + 2), *(str + 3));
    return s.num;
}

static inline uint32_t hexcode_to_uint32(const char *str)
{
    uint32_accessor s;
    s.byte.hhi = hexascii2_to_byte(*(str + 0), *(str + 1));
    s.byte.hlo = hexascii2_to_byte(*(str + 2), *(str + 3));
    s.byte.lhi = hexascii2_to_byte(*(str + 4), *(str + 5));
    s.byte.llo = hexascii2_to_byte(*(str + 6), *(str + 7));
    return s.num;
}

static inline uint64_t hexcode_to_uint64(const char *str)
{
    uint64_accessor s;
    s.byte.hhhi = hexascii2_to_byte(*(str + 0), *(str + 1));
    s.byte.hhlo = hexascii2_to_byte(*(str + 2), *(str + 3));
    s.byte.hlhi = hexascii2_to_byte(*(str + 4), *(str + 5));
    s.byte.hllo = hexascii2_to_byte(*(str + 6), *(str + 7));
    s.byte.lhhi = hexascii2_to_byte(*(str + 8), *(str + 9));
    s.byte.lhlo = hexascii2_to_byte(*(str + 10), *(str + 11));
    s.byte.llhi = hexascii2_to_byte(*(str + 12), *(str + 13));
    s.byte.lllo = hexascii2_to_byte(*(str + 14), *(str + 15));
    return s.num;
}

static inline int8_t hexcode_to_int8(const char *ptr)
{
    return static_cast<int8_t>(hexcode_to_uint8(ptr));
}

static inline int16_t hexcode_to_int16(const char *ptr)
{
    return static_cast<int16_t>(hexcode_to_uint16(ptr));
}

static inline int32_t hexcode_to_int32(const char *ptr)
{
    return static_cast<int32_t>(hexcode_to_uint32(ptr));
}

static inline int64_t hexcode_to_int64(const char *ptr)
{
    return static_cast<int64_t>(hexcode_to_uint64(ptr));
}

static inline void uint4_hcode(char *ptr, uint8_t src)
{
    *ptr = halfbyte_to_hexascii(src);
}

static inline void uint8_hcode(char *ptr, uint8_t src)
{
    uint8_accessor acs;
    acs.num = src;
    *(ptr + 1) = halfbyte_to_hexascii(acs.half.l);
    *(ptr + 0) = halfbyte_to_hexascii(acs.half.h);
}

static inline void uint16_hcode(char *ptr, uint16_t src)
{
    uint16_accessor acs;
    acs.num = src;
    *(ptr + 3) = halfbyte_to_hexascii(acs.half.lo_l);
    *(ptr + 2) = halfbyte_to_hexascii(acs.half.lo_h);
    *(ptr + 1) = halfbyte_to_hexascii(acs.half.hi_l);
    *(ptr + 0) = halfbyte_to_hexascii(acs.half.hi_h);
}

static inline void uint32_hcode(char *ptr, uint32_t src)
{
    uint32_accessor acs;
    acs.num = src;
    *(ptr + 7) = halfbyte_to_hexascii(acs.half.llo_l);
    *(ptr + 6) = halfbyte_to_hexascii(acs.half.llo_h);
    *(ptr + 5) = halfbyte_to_hexascii(acs.half.lhi_l);
    *(ptr + 4) = halfbyte_to_hexascii(acs.half.lhi_h);
    *(ptr + 3) = halfbyte_to_hexascii(acs.half.hlo_l);
    *(ptr + 2) = halfbyte_to_hexascii(acs.half.hlo_h);
    *(ptr + 1) = halfbyte_to_hexascii(acs.half.hhi_l);
    *(ptr + 0) = halfbyte_to_hexascii(acs.half.hhi_h);
}

static inline void uint64_hcode(char *ptr, uint64_t src)
{
    uint64_accessor acs;
    acs.num = src;
    *(ptr + 15) = halfbyte_to_hexascii(acs.half.lllo_l);
    *(ptr + 14) = halfbyte_to_hexascii(acs.half.lllo_h);
    *(ptr + 13) = halfbyte_to_hexascii(acs.half.llhi_l);
    *(ptr + 12) = halfbyte_to_hexascii(acs.half.llhi_h);
    *(ptr + 11) = halfbyte_to_hexascii(acs.half.lhlo_l);
    *(ptr + 10) = halfbyte_to_hexascii(acs.half.lhlo_h);
    *(ptr + 9) = halfbyte_to_hexascii(acs.half.lhhi_l);
    *(ptr + 8) = halfbyte_to_hexascii(acs.half.lhhi_h);
    *(ptr + 7) = halfbyte_to_hexascii(acs.half.hllo_l);
    *(ptr + 6) = halfbyte_to_hexascii(acs.half.hllo_h);
    *(ptr + 5) = halfbyte_to_hexascii(acs.half.hlhi_l);
    *(ptr + 4) = halfbyte_to_hexascii(acs.half.hlhi_h);
    *(ptr + 3) = halfbyte_to_hexascii(acs.half.hhlo_l);
    *(ptr + 2) = halfbyte_to_hexascii(acs.half.hhlo_h);
    *(ptr + 1) = halfbyte_to_hexascii(acs.half.hhhi_l);
    *(ptr + 0) = halfbyte_to_hexascii(acs.half.hhhi_h);
}

static inline uint8_t deccode1(const char *ptr)
{
    return hexascii_to_halfbyte(*ptr);
}

static inline uint8_t deccode2(const char *ptr)
{
    return hexascii_to_halfbyte(*ptr) * 10 + hexascii_to_halfbyte(*(ptr + 1));
}

static inline uint8_t hex2hbyte(char c)
{
    return (uint8_t)(c <= '9' ? c - '0' : c - 'A' + 10);
}

static inline char hbyte2hex(uint8_t n)
{
    return n < 10 ? '0' + n : 'A' - 10 + n;
}

static inline uint8_t dec2hbyte(char c)
{
    return (uint8_t)(c - '0');
}

static inline char hbyte2dec(uint8_t n)
{
    return '0' + n;
}

static inline uint8_t hex2byte(const char *str)
{
    uint8_accessor ret;
    ret.half.h = hex2hbyte(*str++);
    ret.half.l = hex2hbyte(*str);
    return ret.num;
}

static inline void byte2hex(char *buf, uint8_t src)
{
    uint8_accessor acs;
    acs.num = src;
    *buf++ = hbyte2hex(acs.half.h);
    *buf++ = hbyte2hex(acs.half.l);
}

static inline void hex2bytes(void *num, const char *hex, int numlen)
{
    char *start = (char *)num;
    char *stop = (char *)num + numlen;
    for (char *ptr = start; ptr != stop; ++ptr)
    {
        *ptr = hex2byte(hex);
        hex += 2;
    };
}

static inline void hex2bytes_swap(void *num, const char *hex, int numlen)
{
    char *start = (char *)num + numlen - 1;
    char *stop = (char *)num - 1;
    for (char *ptr = start; ptr != stop; --ptr)
    {
        *ptr = hex2byte(hex);
        hex += 2;
    };
}

static inline void bytes2hex(char *hex, const void *num, int numlen)
{
    char *start = (char *)num;
    char *stop = (char *)num + numlen;
    for (char *ptr = start; ptr != stop; ++ptr)
    {
        byte2hex(hex, *ptr);
        hex += 2;
    };
}

static inline void bytes2hex_swap(char *hex, const void *num, int numlen)
{
    char *start = (char *)num + numlen - 1;
    char *stop = (char *)num - 1;
    for (char *ptr = start; ptr != stop; --ptr)
    {
        byte2hex(hex, *ptr);
        hex += 2;
    };
}

#endif