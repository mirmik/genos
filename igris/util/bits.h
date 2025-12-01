#ifndef IGRIS_UTIL_BITS_H
#define IGRIS_UTIL_BITS_H

#include <igris/compiler.h>
#include <stdint.h>

#ifdef _MSC_VER
#include <intrin.h>

static inline uint32_t ctz(uint32_t value)
{
    unsigned long trailing_zero = 0;

    if (_BitScanForward(&trailing_zero, value))
    {
        return trailing_zero;
    }
    else
    {
        // This is undefined, I better choose 32 than 0
        return 32;
    }
}

static inline uint32_t clz(uint32_t value)
{
    unsigned long leading_zero = 0;

    if (_BitScanReverse(&leading_zero, value))
    {
        return 31 - leading_zero;
    }
    else
    {
        // Same remarks as above
        return 32;
    }
}
#else

#define ctz(v) __builtin_ctz(v)
#define clz(v) __builtin_clz(v)

#endif

//Установить все биты в переменной.
#define bits_set_all(a)                                                        \
    {                                                                          \
        a = -1;                                                                \
    }

//Очистить все биты в переменной.
#define bits_clr_all(a)                                                        \
    {                                                                          \
        a = 0;                                                                 \
    }

//#define bits_lvl_mask(a,m,l) (a = (l) ? ((a) | (m)) : ((a) & (~(m))))

//Установить все биты маски в переменную.
#define bits_set(a, m)                                                         \
    {                                                                          \
        a |= (m);                                                              \
    }

//Снять все биты маски в переменной.
#define bits_clr(a, m)                                                         \
    {                                                                          \
        a &= ~(m);                                                             \
    }

//Установить все биты маски в переменную согласно уровню.
#define bits_lvl(a, b, c)                                                      \
    {                                                                          \
        a = ((c) ? ((a) | (b)) : ((a) & (~(b))));                              \
    }
#define bits_rev(a)                                                            \
    {                                                                          \
        a = ~(a);                                                              \
    }

// Вернуть биты, определяемые маской.
// Немаскированные биты равны 0
#define bits_mask(a, m) ((a) & (m))

// Вернуть биты, определяемые инверсией маски.
// Немаскированные биты равны 0
#define bits_invmask(a, m) ((a) & (~(m)))

#define bits_mask_rev(a, m)                                                    \
    {                                                                          \
        a ^= (m);                                                              \
    }
#define bits_mask_eq(a, m, b) (((a) & (m)) == (b))

// Присвоить биты с предварительным наложением маски.
// Немаскированные биты не изменяются.
#define bits_assign(a, m, b) (a = bits_invmask((a), (m)) | bits_mask((b), (m)))
#define bits_assign_bias(a, m, b, s)                                           \
    (bits_assign(a, ((m) << (s)), ((b) << (s))))

#define bits_mask_bias(a, m, s) (bits_mask((a), (m)) >> (s))

// Размножить cllen битов переменной clone в соответствии с шаблоном input.
// Например:
// для
// clone: 0b...XXXXX101
// cllen: 3
// input: 0b1011
//
// return 0b101000101101:
//            1  0  1  1
static inline uint32_t
bits_multimap(uint16_t input, uint32_t clone, uint8_t cllen)
{
    uint32_t ret = 0;

    while (input)
    {
        uint8_t nbit = ctz(input);
        ret |= (clone << (cllen * nbit));
        input &= ~(1 << nbit);
    }

    return ret;
}

// Создать маску повторяющихся наборов по cllen битов,
// используя input в качестве карты.
// Например:
// для
// cllen: 3
// input: 0b1011
//
// return 0b111000111111:
//            1  0  1  1

static inline uint32_t bits_multimap_fullset(uint16_t input, uint8_t cllen)
{
    uint32_t clone = 0;

    for (int i = 0; i < cllen; i++)
    {
        clone = (clone << 1) + 1;
    };

    return bits_multimap(input, clone, cllen);
}

// Присвоит переменной target результат bits_multimap (см. выше),
// изменяя только биты, попадающие под маску.
#define bits_masked_assign_multimap(target, input, clone, cllen)               \
    ({                                                                         \
        uint32_t macro_value_a = bits_multimap(input, clone, cllen);           \
        uint32_t macro_value_b = bits_multimap_fullset(input, cllen);          \
        bits_assign(target, macro_value_b, macro_value_a);                     \
    })

#endif
