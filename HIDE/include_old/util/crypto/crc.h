#ifndef GENOS_CRYPTO_H
#define GENOS_CRYPTO_H

#include "string.h"

/*
  Name  : CRC-8
  Poly  : 0x31    x^8 + x^5 + x^4 + 1
  Init  : 0xFF
  Revert: false
  XorOut: 0x00
  Check : 0xF7 ("123456789")
  MaxLen: 15 байт(127 бит) - обнаружение
    одинарных, двойных, тройных и всех нечетных ошибок
*/
class Crc8Functor {
    unsigned char crc = 0xFF;
    unsigned char CRC_Polynom = 0x31;
public:
    void operator()(char byte) {
        crc ^= byte;
        for (uint8_t i = 0; i < 8; i++) {
          crc = (crc & (1<<7)) 
          ? ((crc << 1) ^ CRC_Polynom) 
          : (crc << 1);
        }
    };

    operator unsigned char () {
        return crc;
    }

    Crc8Functor() {};
};

static unsigned char crc8(unsigned char *pcBlock, unsigned int len)
{
    unsigned char crc = 0xFF;
    unsigned int i;

    while (len--)
    {
        crc ^= *pcBlock++;
        for (i = 0; i < 8; i++)
            crc = crc & 0x80 ? (crc << 1) ^ 0x31 : crc << 1;
    }
    return crc;
}

static unsigned int strcrc8(const char *str)
{
    unsigned char* block = (unsigned char*) str; 
    unsigned int len = strlen(str);
    unsigned char crc = 0xFF;
    unsigned int i;

    while (len--)
    {
        crc ^= *block++;
        for (i = 0; i < 8; i++)
            crc = crc & 0x80 ? (crc << 1) ^ 0x31 : crc << 1;
    }
    return crc & 0xFF;
}

#endif 