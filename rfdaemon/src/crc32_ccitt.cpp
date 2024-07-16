#include "crc32_ccitt.h"

uint32_t crc32_ccitt(const void *data, uint32_t length, uint32_t crc_init)
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
