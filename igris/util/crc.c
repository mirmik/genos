#include <igris/util/crc.h>

// Dow-CRC using polynomial X^8 + X^5 + X^4 + X^0
// Tiny 2x16 entry CRC table created by Arjen Lentz
// See
// http://lentz.com.au/blog/calculating-crc-with-a-tiny-32-entry-lookup-table
static const uint8_t dscrc2x16_table[] = {
    0x00, 0x5E, 0xBC, 0xE2, 0x61, 0x3F, 0xDD, 0x83, 0xC2, 0x9C, 0x7E,
    0x20, 0xA3, 0xFD, 0x1F, 0x41, 0x00, 0x9D, 0x23, 0xBE, 0x46, 0xDB,
    0x65, 0xF8, 0x8C, 0x11, 0xAF, 0x32, 0xCA, 0x57, 0xE9, 0x74};

// Compute a Dallas Semiconductor 8 bit CRC. These show up in the ROM
// and the registers.  (Use tiny 2x16 entry CRC table)
uint8_t igris_crc8_table(const uint8_t *addr, uint8_t len, uint8_t crc_init)
{
    uint8_t crc = crc_init;

    while (len--)
    {
        crc = *addr++ ^ crc; // just re-using crc as intermediate
        crc = dscrc2x16_table[crc & 0x0f] ^
              dscrc2x16_table[16 + ((crc >> 4) & 0x0f)];
    }

    return crc;
}

//
// Compute a Dallas Semiconductor 8 bit CRC directly.
// this is much slower, but a little smaller, than the lookup table.
//
uint8_t igris_crc8(const void *data, uint8_t len, uint8_t crc_init)
{
    uint8_t *addr = (uint8_t *)data;
    uint8_t crc = crc_init;

    while (len--)
    {
        uint8_t inbyte = *addr++;
        for (uint8_t i = 8; i; i--)
        {
            uint8_t mix = (crc ^ inbyte) & 0x01;
            crc >>= 1;
            if (mix)
                crc ^= 0x8C;
            inbyte >>= 1;
        }
    }
    return crc;
}

uint16_t igris_crc16(const void *data, uint16_t length, uint16_t crc_init)
{
    const uint8_t *data_p = (const uint8_t *)data;
    uint8_t x;
    uint16_t crc = crc_init;

    while (length--)
    {
        x = crc >> 8 ^ *data_p++;
        x ^= x >> 4;
        crc = (crc << 8) ^ ((uint16_t)(x << 12)) ^ ((uint16_t)(x << 5)) ^
              ((uint16_t)x);
    }
    return crc;
}

uint8_t igris_mmc_crc7(const uint8_t *message, const uint8_t length)
{
    const uint8_t poly = 0x89;
    uint8_t crc = 0;

    for (unsigned i = 0; i < length; i++)
    {
        crc ^= message[i];
        for (int j = 0; j < 8; j++)
        {
            // crc = crc & 0x1 ? (crc >> 1) ^ poly : crc >> 1;
            crc = (crc & 0x80u) ? ((crc << 1) ^ (poly << 1)) : (crc << 1);
        }
    }
    // return crc;
    return crc >> 1;
}

uint32_t igris_crc32(const void *data, uint32_t length, uint32_t crc_init)
{

    // Nibble lookup table for 0x04C11DB7 polynomial

    static const uint32_t crcTable[16] = {
        0x00000000, 0x04C11DB7, 0x09823B6E, 0x0D4326D9, 0x130476DC, 0x17C56B6B,
        0x1A864DB2, 0x1E475005, 0x2608EDB8, 0x22C9F00F, 0x2F8AD6D6, 0x2B4BCB61,
        0x350C9B64, 0x31CD86D3, 0x3C8EA00A, 0x384FBDBD};

    uint32_t crc = crc_init;

    const uint32_t *pData = (const uint32_t *)data;
    uint32_t bodySize = length / 4;
    uint32_t tailSize = length % 4;

    for (uint32_t i = 0; i < bodySize; i++)
    {

        crc = crc ^ pData[i];
        crc = (crc << 4) ^ crcTable[crc >> 28];
        crc = (crc << 4) ^ crcTable[crc >> 28];
        crc = (crc << 4) ^ crcTable[crc >> 28];
        crc = (crc << 4) ^ crcTable[crc >> 28];
        crc = (crc << 4) ^ crcTable[crc >> 28];
        crc = (crc << 4) ^ crcTable[crc >> 28];
        crc = (crc << 4) ^ crcTable[crc >> 28];
        crc = (crc << 4) ^ crcTable[crc >> 28];
    }

    if (tailSize)
    {
        crc = crc ^ (pData[bodySize] & ((1 << tailSize * 8) - 1));
        crc = (crc << 4) ^ crcTable[crc >> 28];
        crc = (crc << 4) ^ crcTable[crc >> 28];
        crc = (crc << 4) ^ crcTable[crc >> 28];
        crc = (crc << 4) ^ crcTable[crc >> 28];
        crc = (crc << 4) ^ crcTable[crc >> 28];
        crc = (crc << 4) ^ crcTable[crc >> 28];
        crc = (crc << 4) ^ crcTable[crc >> 28];
        crc = (crc << 4) ^ crcTable[crc >> 28];
    }

    return crc;
}

#if 0
uint8_t crc8(const void *data, uint8_t length, uint8_t crc_init) 
{
    return igris_crc8(data, length, crc_init);
}

uint16_t crc16(const void *data, uint16_t length, uint16_t crc_init) 
{
    return igris_crc16(data, length, crc_init);
}

uint32_t crc32(const void *data, uint32_t length, uint32_t crc_init) 
{
    return igris_crc32(data, length, crc_init);
}

uint8_t mmc_crc7(const uint8_t *message, const uint8_t length) 
{
    return igris_mmc_crc7(message, length);
}

uint8_t crc8_table(const uint8_t *addr, uint8_t len, uint8_t crc_init) 
{
    return igris_crc8_table(addr, len, crc_init);
}
#endif
