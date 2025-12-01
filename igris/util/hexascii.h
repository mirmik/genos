#ifndef NOS_UTIL_HEXASCII_H
#define NOS_UTIL_HEXASCII_H

/**
@file
*/

#include <igris/compiler.h>
#include <igris/util/access.h>
#include <stdint.h>

__BEGIN_DECLS

void hexascii_encode(const void *indata, int size, void *out);
void hexascii_decode(const void *indata, int size, void *out);

__END_DECLS

CONSTEXPR static inline uint8_t hex2half(char c)
{
    return (uint8_t)(c <= '9' ? c - '0' : c - 'A' + 10);
}

CONSTEXPR static inline char half2hex(uint8_t n)
{
    return (char)(n < 10 ? '0' + n : 'A' - 10 + n);
}

CONSTEXPR static inline uint8_t hex2byte(char hi, char lo)
{
    return (uint8_t)((hex2half(hi) << 4) + hex2half(lo));
}

static inline uint8_t hex_to_uint8(const char *hex)
{
    uint8_t out;

    out = hex2byte(*(hex + 0), *(hex + 1));

    return out;
}

static inline uint16_t hex_to_uint16(const char *hex)
{
    uint16_t out;

    UINT16_HI(out) = hex2byte(*(hex + 0), *(hex + 1));
    UINT16_LO(out) = hex2byte(*(hex + 2), *(hex + 3));

    return out;
}

static inline uint32_t hex_to_uint32(const char *hex)
{
    uint32_t out;

    UINT32_HHI(out) = hex2byte(*(hex + 0), *(hex + 1));
    UINT32_HLO(out) = hex2byte(*(hex + 2), *(hex + 3));
    UINT32_LHI(out) = hex2byte(*(hex + 4), *(hex + 5));
    UINT32_LLO(out) = hex2byte(*(hex + 6), *(hex + 7));

    return out;
}

static inline uint64_t hex_to_uint64(const char *hex)
{
    uint64_t out;

    UINT64_HHHI(out) = hex2byte(*(hex + 0), *(hex + 1));
    UINT64_HHLO(out) = hex2byte(*(hex + 2), *(hex + 3));
    UINT64_HLHI(out) = hex2byte(*(hex + 4), *(hex + 5));
    UINT64_HLLO(out) = hex2byte(*(hex + 6), *(hex + 7));
    UINT64_LHHI(out) = hex2byte(*(hex + 8), *(hex + 9));
    UINT64_LHLO(out) = hex2byte(*(hex + 10), *(hex + 11));
    UINT64_LLHI(out) = hex2byte(*(hex + 12), *(hex + 13));
    UINT64_LLLO(out) = hex2byte(*(hex + 14), *(hex + 15));

    return out;
}

static inline void uint8_to_hex(char *hex, uint8_t in)
{
    *hex++ = half2hex(HIHALF(in));
    *hex++ = half2hex(LOHALF(in));
}

static inline void uint16_to_hex(char *hex, uint16_t in)
{
    *hex++ = half2hex(HIHALF(UINT16_HI(in)));
    *hex++ = half2hex(LOHALF(UINT16_HI(in)));
    *hex++ = half2hex(HIHALF(UINT16_LO(in)));
    *hex++ = half2hex(LOHALF(UINT16_LO(in)));
}

static inline void uint32_to_hex(char *hex, uint32_t in)
{
    *hex++ = half2hex(HIHALF(UINT32_HHI(in)));
    *hex++ = half2hex(LOHALF(UINT32_HHI(in)));
    *hex++ = half2hex(HIHALF(UINT32_HLO(in)));
    *hex++ = half2hex(LOHALF(UINT32_HLO(in)));
    *hex++ = half2hex(HIHALF(UINT32_LHI(in)));
    *hex++ = half2hex(LOHALF(UINT32_LHI(in)));
    *hex++ = half2hex(HIHALF(UINT32_LLO(in)));
    *hex++ = half2hex(LOHALF(UINT32_LLO(in)));
}

static inline void uint64_to_hex(char *hex, uint64_t in)
{
    *hex++ = half2hex(HIHALF(UINT64_HHHI(in)));
    *hex++ = half2hex(LOHALF(UINT64_HHHI(in)));
    *hex++ = half2hex(HIHALF(UINT64_HHLO(in)));
    *hex++ = half2hex(LOHALF(UINT64_HHLO(in)));
    *hex++ = half2hex(HIHALF(UINT64_HLHI(in)));
    *hex++ = half2hex(LOHALF(UINT64_HLHI(in)));
    *hex++ = half2hex(HIHALF(UINT64_HLLO(in)));
    *hex++ = half2hex(LOHALF(UINT64_HLLO(in)));
    *hex++ = half2hex(HIHALF(UINT64_LHHI(in)));
    *hex++ = half2hex(LOHALF(UINT64_LHHI(in)));
    *hex++ = half2hex(HIHALF(UINT64_LHLO(in)));
    *hex++ = half2hex(LOHALF(UINT64_LHLO(in)));
    *hex++ = half2hex(HIHALF(UINT64_LLHI(in)));
    *hex++ = half2hex(LOHALF(UINT64_LLHI(in)));
    *hex++ = half2hex(HIHALF(UINT64_LLLO(in)));
    *hex++ = half2hex(LOHALF(UINT64_LLLO(in)));
}

#endif
