/**
    @file igris/protocols/gstuff.h
*/

#ifndef GSTUFF2_H
#define GSTUFF2_H

#include <cstdint>
#include <igris/buffer.h>
#include <igris/compiler.h>
#include <igris/datastruct/sline.h>
#include <igris/iovec.h>
#include <vector>

#define GSTUFF_START ((char)0xA8)
#define GSTUFF_STOP ((char)0xB2)
#define GSTUFF_STUB ((char)0xC5)
#define GSTUFF_STUB_START ((char)0x8A)
#define GSTUFF_STUB_STOP ((char)0x2B)
#define GSTUFF_STUB_STUB ((char)0x5C)

#define GSTUFF_CRC_ERROR -1
#define GSTUFF_OVERFLOW -2
#define GSTUFF_STUFFING_ERROR -3
#define GSTUFF_ALGORITHM_ERROR -4

#define GSTUFF_CONTINUE 0
#define GSTUFF_NEWPACKAGE 1
#define GSTUFF_FORCE_RESTART 2
#define GSTUFF_GARBAGE 3

class gstuff_autorecv
{
    struct sline line = {};
    uint8_t crc = 0;
    uint8_t state = 0;

public:
    gstuff_autorecv() = default;
    gstuff_autorecv(uint8_t *buf, int len);
    void init(uint8_t *buf, int len);
    void setbuf(uint8_t *buf, int len)
    {
        init(buf, len);
    }
    void reset();
    int newchar(char c);

    const char *cstr()
    {
        return sline_getline(&line);
    }
    size_t size()
    {
        return sline_size(&line);
    }
};

/**
    Собрать пакет gstuff.
    @param data - входной буффер
    @param size - длина входного буффер
    @param outdata - выходной буффер (рекомендованная длина 2*size+2)
    @return результирующая длина пакета.
 */
int gstuffing(const char *data, size_t size, char *outdata);
int gstuffing_v(struct iovec *vec, size_t n, char *outdata);
int gstuff_byte(char c, char *outdata);

std::vector<uint8_t> gstuffing_v(struct iovec *vec, size_t n);
std::vector<uint8_t> gstuffing(igris::buffer buf);

#endif
