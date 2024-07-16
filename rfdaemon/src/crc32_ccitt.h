#ifndef CRC32_CCITT_H
#define CRC32_CCITT_H

#include <stdint.h>

__BEGIN_DECLS

uint32_t crc32_ccitt(const void *data, uint32_t length, uint32_t crc_init);

__END_DECLS

#endif
