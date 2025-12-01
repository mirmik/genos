#include "crc32_ccitt.h"

uint32_t crc32_ccitt(const void *data, uint32_t length, uint32_t crc_init)
{
    static const uint32_t crcTable[16] = {
        0x00000000, 0x04C11DB7, 0x09823B6E, 0x0D4326D9, 0x130476DC, 0x17C56B6B,
        0x1A864DB2, 0x1E475005, 0x2608EDB8, 0x22C9F00F, 0x2F8AD6D6, 0x2B4BCB61,
        0x350C9B64, 0x31CD86D3, 0x3C8EA00A, 0x384FBDBD};

    if (data == nullptr || length == 0)
        return crc_init;

    const uint8_t *p = static_cast<const uint8_t *>(data);
    uint32_t crc = crc_init;

    // по 4 байта
    while (length >= 4)
    {
        uint32_t word = (static_cast<uint32_t>(p[0]) << 24) |
                        (static_cast<uint32_t>(p[1]) << 16) |
                        (static_cast<uint32_t>(p[2]) << 8) |
                        (static_cast<uint32_t>(p[3]) << 0);

        crc ^= word;
        for (int i = 0; i < 8; ++i)
        {
            crc = (crc << 4) ^ crcTable[crc >> 28];
        }

        p += 4;
        length -= 4;
    }

    // хвост 1–3 байта
    if (length > 0)
    {
        uint32_t word = 0;
        for (uint32_t i = 0; i < length; ++i)
        {
            word |= static_cast<uint32_t>(p[i]) << ((3 - i) * 8);
        }

        crc ^= word;
        for (int i = 0; i < 8; ++i)
        {
            crc = (crc << 4) ^ crcTable[crc >> 28];
        }
    }

    return crc;
}
