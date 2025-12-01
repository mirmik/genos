#ifndef IGRIS_UTIL_CRC_H
#define IGRIS_UTIL_CRC_H

#include <igris/compiler.h>
#include <stdint.h>

__BEGIN_DECLS
static inline void igris_strmcrc8(uint8_t *crc, char c)
{
    *crc ^= (uint8_t)c;
    for (int i = 0; i < 8; i++)
        *crc = *crc & 0x80 ? ((uint8_t)(((uint8_t)(*crc << 1)) ^ 0x31))
                           : ((uint8_t)(*crc << 1));
}

uint8_t igris_crc8(const void *data, uint8_t length, uint8_t crc_init);
uint16_t igris_crc16(const void *data, uint16_t length, uint16_t crc_init);
uint32_t igris_crc32(const void *data, uint32_t length, uint32_t crc_init);

uint8_t igris_mmc_crc7(const uint8_t *message, const uint8_t length);
uint8_t igris_crc8_table(const uint8_t *addr, uint8_t len, uint8_t crc_init);

#if 0
__attribute__((deprecated,weak)) uint8_t crc8(const void *data, uint8_t length,
                                         uint8_t crc_init);

__attribute__((deprecated,weak)) uint16_t crc16(const void *data, uint16_t length,
                                           uint16_t crc_init);

__attribute__((deprecated,weak)) uint32_t crc32(const void *data, uint32_t length,
                                           uint32_t crc_init);

__attribute__((deprecated,weak)) uint8_t mmc_crc7(const uint8_t *message,
                                             const uint8_t length);

__attribute__((deprecated,weak)) uint8_t crc8_table(const uint8_t *addr, uint8_t len,
                                               uint8_t crc_init);
#endif
__END_DECLS

#endif
