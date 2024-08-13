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

#define GSTUFF_START_V1 ((char)0xA8)
#define GSTUFF_STOP_V1 ((char)0xB2)
#define GSTUFF_STUB_V1 ((char)0xC5)
#define GSTUFF_STUB_START_V1 ((char)0x8A)
#define GSTUFF_STUB_STOP_V1 ((char)0x2B)
#define GSTUFF_STUB_STUB_V1 ((char)0x5C)

#define GSTUFF_START_V0 ((char)0xAC)
#define GSTUFF_STUB_V0 ((char)0xAD)
#define GSTUFF_STUB_START_V0 ((char)0xAE)
#define GSTUFF_STUB_STUB_V0 ((char)0xAF)


#define GSTUFF_CRC_ERROR -1
#define GSTUFF_OVERFLOW -2
#define GSTUFF_STUFFING_ERROR -3
#define GSTUFF_ALGORITHM_ERROR -4

#define GSTUFF_CONTINUE 0
#define GSTUFF_NEWPACKAGE 1
#define GSTUFF_FORCE_RESTART 2
#define GSTUFF_GARBAGE 3

struct gstuff_context 
{
    char GSTUFF_START = GSTUFF_START_V1;
    char GSTUFF_STOP = GSTUFF_STOP_V1;
    char GSTUFF_STUB = GSTUFF_STUB_V1;
    char GSTUFF_STUB_START = GSTUFF_STUB_START_V1;
    char GSTUFF_STUB_STOP = GSTUFF_STUB_STOP_V1;
    char GSTUFF_STUB_STUB = GSTUFF_STUB_STUB_V1;
};

static inline gstuff_context gstuff_context_v0() 
{
    return gstuff_context 
    {
        GSTUFF_START_V0, GSTUFF_START_V0,
        GSTUFF_STUB_V0,
        GSTUFF_STUB_START_V0, GSTUFF_STUB_START_V0,
        GSTUFF_STUB_STUB_V0
    };
} 

class gstuff_autorecv
{
    struct sline line = {};
    uint8_t crc = 0;
    uint8_t state = 0;
    gstuff_context ctx;

public:
    gstuff_autorecv(gstuff_context ctx) 
        : ctx(ctx)
    {}
    gstuff_autorecv(uint8_t *buf, int len, gstuff_context ctx);
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
int gstuffing(const char *data, size_t size, char *outdata, 
    const gstuff_context& ctx);
std::vector<uint8_t> gstuffing(igris::buffer buf, 
    const gstuff_context& ctx);

/**
    Собрать пакет gstuff.
    @param vec - входной вектор
    @param n - количество элементов в векторе
    @param outdata - выходной буффер (рекомендованная длина 2*size+2)
    @return результирующая длина пакета.
 */
int gstuffing_v(struct iovec *vec, size_t n, char *outdata, 
    const gstuff_context& ctx);
std::vector<uint8_t> gstuffing_v(struct iovec *vec, size_t n, 
    const gstuff_context& ctx);

/**
    Записывает в outdata трансляцию байта согласно протокола gstuff.
    возвращает количество записанных байт.
*/
int gstuff_byte(char c, char *outdata, 
    const gstuff_context& ctx);

#endif
