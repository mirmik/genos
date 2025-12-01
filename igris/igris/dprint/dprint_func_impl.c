/**
 * Little endian
 */

#include <igris/dprint/dprint.h>
#include <igris/util/ctype.h>
#include <stdarg.h>
#include <stdint.h>

void debug_printbin_uint4(uint8_t b)
{
    debug_putchar((b & 0x08) ? '1' : '0');
    debug_putchar((b & 0x04) ? '1' : '0');
    debug_putchar((b & 0x02) ? '1' : '0');
    debug_putchar((b & 0x01) ? '1' : '0');
}

void debug_printbin_uint8(uint8_t b)
{
    debug_putchar((b & 0x80) ? '1' : '0');
    debug_putchar((b & 0x40) ? '1' : '0');
    debug_putchar((b & 0x20) ? '1' : '0');
    debug_putchar((b & 0x10) ? '1' : '0');
    debug_putchar((b & 0x08) ? '1' : '0');
    debug_putchar((b & 0x04) ? '1' : '0');
    debug_putchar((b & 0x02) ? '1' : '0');
    debug_putchar((b & 0x01) ? '1' : '0');
}

void debug_printbin_uint16(uint16_t a)
{
    uint8_t *p = (uint8_t *)&a;
    debug_printbin_uint8(*(p + 1));
    debug_printbin_uint8(*(p + 0));
}

void debug_printbin_uint32(uint32_t a)
{
    uint8_t *p = (uint8_t *)&a;
    debug_printbin_uint8(*(p + 3));
    debug_printbin_uint8(*(p + 2));
    debug_printbin_uint8(*(p + 1));
    debug_printbin_uint8(*(p + 0));
}

void debug_printbin_uint64(uint64_t a)
{
    uint8_t *p = (uint8_t *)&a;
    debug_printbin_uint8(*(p + 7));
    debug_printbin_uint8(*(p + 6));
    debug_printbin_uint8(*(p + 5));
    debug_printbin_uint8(*(p + 4));
    debug_printbin_uint8(*(p + 3));
    debug_printbin_uint8(*(p + 2));
    debug_printbin_uint8(*(p + 1));
    debug_printbin_uint8(*(p + 0));
}

void debug_printdec_uint64(uint64_t x)
{
    char c[24];
    char *end_buf = c + 24;
    char *ptr = end_buf;

    if (x == 0)
        debug_putchar('0');

    *--ptr = '\0';

    for (; x != 0; x /= 10)
    {
        *--ptr = ((x % 10) + '0');
    }

    debug_print(ptr);
}

void debug_printhex_uint4(uint8_t b)
{
    uint8_t c = b < 10 ? b + '0' : b + 'A' - 10;
    debug_putchar(c);
}

void debug_printhex_uint8(uint8_t b)
{
    debug_printhex_uint4((b & 0xF0) >> 4);
    debug_printhex_uint4(b & 0x0F);
}

void debug_printhex_n(uint8_t *arg, int n)
{
    uint8_t *p = arg + n;

    while (n--)
    {
        debug_printhex_uint8(*--p);
    }
}

void debug_printhex_char(char arg)
{
    debug_printhex_uint8(arg);
}

void debug_printhex_unsigned_char(unsigned char arg)
{
    debug_printhex_uint8(arg);
}
void debug_printhex_unsigned_short(unsigned short arg)
{
    debug_printhex_n((uint8_t *)&arg, sizeof(arg));
}
void debug_printhex_unsigned_int(unsigned int arg)
{
    debug_printhex_n((uint8_t *)&arg, sizeof(arg));
}
void debug_printhex_unsigned_long(unsigned long arg)
{
    debug_printhex_n((uint8_t *)&arg, sizeof(arg));
}
void debug_printhex_unsigned_long_long(unsigned long long arg)
{
    debug_printhex_n((uint8_t *)&arg, sizeof(arg));
}

void debug_printhex_signed_char(signed char arg)
{
    debug_printhex_uint8(arg);
}
void debug_printhex_signed_short(signed short arg)
{
    debug_printhex_n((uint8_t *)&arg, sizeof(arg));
}
void debug_printhex_signed_int(signed int arg)
{
    debug_printhex_n((uint8_t *)&arg, sizeof(arg));
}
void debug_printhex_signed_long(signed long arg)
{
    debug_printhex_n((uint8_t *)&arg, sizeof(arg));
}
void debug_printhex_signed_long_long(signed long long arg)
{
    debug_printhex_n((uint8_t *)&arg, sizeof(arg));
}

void debug_printhex_uint16(uint16_t a)
{
    uint8_t *p = (uint8_t *)&a;
    debug_printhex_uint8(*(p + 1));
    debug_printhex_uint8(*(p + 0));
}

void debug_printhex_uint32(uint32_t a)
{
    uint8_t *p = (uint8_t *)&a;
    debug_printhex_uint8(*(p + 3));
    debug_printhex_uint8(*(p + 2));
    debug_printhex_uint8(*(p + 1));
    debug_printhex_uint8(*(p + 0));
}

void debug_printhex_uint64(uint64_t a)
{
    uint8_t *p = (uint8_t *)&a;
    debug_printhex_uint8(*(p + 7));
    debug_printhex_uint8(*(p + 6));
    debug_printhex_uint8(*(p + 5));
    debug_printhex_uint8(*(p + 4));
    debug_printhex_uint8(*(p + 3));
    debug_printhex_uint8(*(p + 2));
    debug_printhex_uint8(*(p + 1));
    debug_printhex_uint8(*(p + 0));
}

void debug_writehex(const void *ptr, uint16_t size)
{
    uint8_t *_ptr = (uint8_t *)ptr;

    while (size--)
    {
        debug_printhex_uint8(*_ptr++);
    }
}

void debug_writehex_reversed(const void *ptr, uint16_t size)
{
    uint8_t *_ptr = (uint8_t *)ptr + size;

    while (size--)
    {
        debug_printhex_uint8(*--_ptr);
    }
}

void debug_writebin(const void *ptr, uint16_t size)
{
    uint8_t *_ptr = (uint8_t *)ptr;

    while (size--)
    {
        debug_printbin_uint8(*_ptr++);
    }
}

void debug_writebin_reversed(const void *ptr, uint16_t size)
{
    uint8_t *_ptr = (uint8_t *)ptr + size;

    while (size--)
    {
        debug_printbin_uint8(*--_ptr);
    }
}

void debug_printdec_float_prec(float a, int prec)
{
    debug_printdec_double_prec((double)a, prec);
}

static inline unsigned __FLOAT_BITS(float __f)
{
    union
    {
        float __f;
        unsigned __i;
    } __u;
    __u.__f = __f;
    return __u.__i;
}
static inline unsigned long long __DOUBLE_BITS(double __f)
{
    union
    {
        double __f;
        unsigned long long __i;
    } __u;
    __u.__f = __f;
    return __u.__i;
}

static inline uint8_t __isnan(double x)
{
    return sizeof(x) == 4
               ? (__FLOAT_BITS((float)x) & 0x7fffffff) > 0x7f800000
               : (__DOUBLE_BITS(x) & (0ULL - 1ULL) >> 1) > 0x7ffULL << 52;
}

static inline uint8_t __isinf(double x)
{
    return sizeof(x) == 4
               ? (__FLOAT_BITS((float)x) & 0x7fffffff) == 0x7f800000
               : (__DOUBLE_BITS(x) & (0ULL - 1ULL) >> 1) == 0x7ffULL << 52;
}

void debug_printdec_double_prec(double a, int prec)
{
    if (__isnan(a))
    {
        debug_print("nan");
        return;
    }

    if (__isinf(a))
    {
        if (a > 0)
            debug_print("+inf");
        else
            debug_print("-inf");
        return;
    }

    if (a < 0)
        debug_putchar('-');

    if (a < 0)
        a = -a;

    uint64_t n = (uint64_t)a;

    debug_printdec_uint64(n);
    debug_putchar('.');

    double o = a - n;

    for (int _iteration = 0; _iteration < prec; ++_iteration)
    {
        o *= 10;

        if ((int)o == 0)
            debug_putchar('0');
    }

    o += 0.5;

    debug_printdec_signed_long_long((long long)o);
}

void debug_printhex_ptr(const void *v)
{
    debug_writehex_reversed(&v, sizeof(uintptr_t));
}

void debug_print(const char *c)
{
    if (c == (const char *)0)
    {
        debug_putchar('N');
        debug_putchar('U');
        debug_putchar('L');
        debug_putchar('L');
    }
    else
    {
        debug_write((char *)c, debug_strlen(c));
    }
}

void debug_print_dump(const void *mem, uint16_t len)
{
    // In real, we doesn't need configure that in debug print library.
    const unsigned int HEXDUMP_COLS = 8;

    for (unsigned int i = 0;
         i <
         len + ((len % HEXDUMP_COLS) ? (HEXDUMP_COLS - len % HEXDUMP_COLS) : 0);
         i++)
    {
        /* print offset */
        if (i % HEXDUMP_COLS == 0)
        {
            debug_write("0x", 2);
            debug_printhex_ptr(i + (char *)mem);
            debug_putchar(':');
        }

        /* print hex data */
        if (i < len)
        {
            debug_printhex_uint8(((char *)mem)[i]);
            debug_putchar(' ');
        }
        else /* end of block, just aligning for ASCII dump */
        {
            debug_print("   ");
        }

        /* print ASCII dump */
        if (i % HEXDUMP_COLS == (HEXDUMP_COLS - 1))
        {
            for (unsigned int j = i - (HEXDUMP_COLS - 1); j <= i; j++)
            {
                if (j >= len) /* end of block, not really printing */
                {
                    debug_putchar(' ');
                }
                else if (igris_isprint(*((char *)mem) + j)) /* printable char */
                {
                    debug_putchar(0xFF & ((char *)mem)[j]);
                }
                else /* other char */
                {
                    debug_putchar('.');
                }
            }

            debug_print_newline();
        }
    }
}

void debug_printdec_signed_char(signed char x)
{
    debug_printdec_signed_long_long((long long)x);
}
void debug_printdec_signed_short(signed short x)
{
    debug_printdec_signed_long_long((long long)x);
}
void debug_printdec_signed_int(signed int x)
{
    debug_printdec_signed_long_long((long long)x);
}
void debug_printdec_signed_long(signed long x)
{
    debug_printdec_signed_long_long((long long)x);
}
void debug_printdec_signed_long_long(signed long long x)
{
    if (x < 0)
    {
        x = -x;
        debug_putchar('-');
    }
    debug_printdec_uint64(x);
}

void debug_printdec_unsigned_char(unsigned char x)
{
    debug_printdec_unsigned_long_long((long long)x);
}
void debug_printdec_unsigned_short(unsigned short x)
{
    debug_printdec_unsigned_long_long((long long)x);
}
void debug_printdec_unsigned_int(unsigned int x)
{
    debug_printdec_unsigned_long_long((long long)x);
}
void debug_printdec_unsigned_long(unsigned long x)
{
    debug_printdec_unsigned_long_long((long long)x);
}
void debug_printdec_unsigned_long_long(unsigned long long x)
{
    debug_printdec_uint64(x);
}

void debug_printdec_uint8(uint8_t x)
{
    debug_printdec_uint64((uint64_t)x);
}
void debug_printdec_uint16(uint16_t x)
{
    debug_printdec_uint64((uint64_t)x);
}
void debug_printdec_uint32(uint32_t x)
{
    debug_printdec_uint64((uint64_t)x);
}

void debug_asmlink_test(void)
{
    debug_putchar('A');
    debug_putchar('B');
    debug_putchar('C');
    debug_putchar('D');
    debug_putchar('E');
    debug_putchar('1');
    debug_putchar('2');
    debug_putchar('3');
    debug_putchar('4');
    debug_putchar('5');
}

uint8_t debug_asmlink_ret8(void)
{
    return 0xFE;
}
uint16_t debug_asmlink_ret16(void)
{
    return 0xFEDC;
}
uint32_t debug_asmlink_ret32(void)
{
    return 0xFEDCBA98;
}
uint64_t debug_asmlink_ret64(void)
{
    return 0xFEDCBA9876543210;
}

void debug_asmlink_args8x1(uint8_t a)
{
    debug_printhex_uint8(a);
    debug_putchar(':');
}

void debug_asmlink_args8x2(uint8_t a, uint8_t b)
{
    debug_printhex_uint8(a);
    debug_putchar(':');
    debug_printhex_uint8(b);
    debug_putchar(':');
}

void debug_asmlink_args8x3(uint8_t a, uint8_t b, uint8_t c)
{
    debug_printhex_uint8(a);
    debug_putchar(':');
    debug_printhex_uint8(b);
    debug_putchar(':');
    debug_printhex_uint8(c);
    debug_putchar(':');
}

void debug_asmlink_args8x4(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
    debug_printhex_uint8(a);
    debug_putchar(':');
    debug_printhex_uint8(b);
    debug_putchar(':');
    debug_printhex_uint8(c);
    debug_putchar(':');
    debug_printhex_uint8(d);
    debug_putchar(':');
}

void debug_asmlink_args16x1(uint16_t a)
{
    debug_printhex_uint16(a);
    debug_putchar(':');
}

void debug_asmlink_args16x2(uint16_t a, uint16_t b)
{
    debug_printhex_uint16(a);
    debug_putchar(':');
    debug_printhex_uint16(b);
    debug_putchar(':');
}

void debug_asmlink_args16x3(uint16_t a, uint16_t b, uint16_t c)
{
    debug_printhex_uint16(a);
    debug_putchar(':');
    debug_printhex_uint16(b);
    debug_putchar(':');
    debug_printhex_uint16(c);
    debug_putchar(':');
}

void debug_asmlink_args16x4(uint16_t a, uint16_t b, uint16_t c, uint16_t d)
{
    debug_printhex_uint16(a);
    debug_putchar(':');
    debug_printhex_uint16(b);
    debug_putchar(':');
    debug_printhex_uint16(c);
    debug_putchar(':');
    debug_printhex_uint16(d);
    debug_putchar(':');
}

void debug_asmlink_args32x1(uint32_t a)
{
    debug_printhex_uint32(a);
    debug_putchar(':');
}

void debug_asmlink_args32x2(uint32_t a, uint32_t b)
{
    debug_printhex_uint32(a);
    debug_putchar(':');
    debug_printhex_uint32(b);
    debug_putchar(':');
}

void debug_asmlink_args32x3(uint32_t a, uint32_t b, uint32_t c)
{
    debug_printhex_uint32(a);
    debug_putchar(':');
    debug_printhex_uint32(b);
    debug_putchar(':');
    debug_printhex_uint32(c);
    debug_putchar(':');
}

void debug_asmlink_args32x4(uint32_t a, uint32_t b, uint32_t c, uint32_t d)
{
    debug_printhex_uint32(a);
    debug_putchar(':');
    debug_printhex_uint32(b);
    debug_putchar(':');
    debug_printhex_uint32(c);
    debug_putchar(':');
    debug_printhex_uint32(d);
    debug_putchar(':');
}

void dprptr(const void *ptr)
{
    debug_printhex_ptr(ptr);
}

void dprptrln(const void *ptr)
{
    debug_printhex_ptr(ptr);
    debug_putchar('\r');
    debug_putchar('\n');
}
