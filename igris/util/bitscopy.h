#ifndef IGRIS_BITSCOPY_H
#define IGRIS_BITSCOPY_H

#include <cstdint>

const int BITS_IN_BYTE = 8;

struct bithandle
{
    uint8_t *byte;
    uint8_t bitno;
};

#define bits_lvl(a, b, c)                                                      \
    {                                                                          \
        a = ((c) ? ((a) | (b)) : ((a) & (~(b))));                              \
    }

static inline void bithandle_inc(struct bithandle *a)
{
    a->bitno++;
    if (a->bitno >= BITS_IN_BYTE)
    {
        a->bitno = 0;
        a->byte++;
    }
}

static inline void bitcopy(struct bithandle a, struct bithandle b)
{
    uint8_t lvl = !!(*b.byte & (1 << b.bitno));
    bits_lvl(*a.byte, (1 << a.bitno), lvl);
}

static inline void bitscopy(uint8_t *dstp, uint8_t dstbitno, uint8_t *srcp,
                            uint8_t srcbitno, uint32_t bitlength)
{
    struct bithandle dst = {dstp, dstbitno};
    struct bithandle src = {srcp, srcbitno};

    while (bitlength--)
    {
        bitcopy(dst, src);
        bithandle_inc(&dst);
        bithandle_inc(&src);
    }
}

#endif