#ifndef IGRIS_USTIL_GRAYCODE_H
#define IGRIS_USTIL_GRAYCODE_H

#include <cstdint>

static inline uint8_t graycode8(uint8_t code) { return code ^ (code >> 1); }
static inline uint16_t graycode16(uint16_t code) { return code ^ (code >> 1); }
static inline uint32_t graycode32(uint32_t code) { return code ^ (code >> 1); }
static inline uint64_t graycode64(uint64_t code) { return code ^ (code >> 1); }

#ifdef __cplusplus

namespace igris
{
    template <class T> static inline T graycode(const T &code)
    {
        return code ^ (code >> 1);
    }
}

#endif

#endif
