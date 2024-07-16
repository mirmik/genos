#include <cmath>
#include <cstdio>
#include <nos/util/numconvert.h>
#include <string.h>

#define MAX_PRECISION (14)
static const long double rounders[MAX_PRECISION + 1] = {
    0.5,               // 0
    0.05,              // 1
    0.005,             // 2
    0.0005,            // 3
    0.00005,           // 4
    0.000005,          // 5
    0.0000005,         // 6
    0.00000005,        // 7
    0.000000005,       // 8
    0.0000000005,      // 9
    0.00000000005,     // 10
    0.000000000005,    // 11
    0.0000000000005,   // 12
    0.00000000000005,  // 13
    0.000000000000005, // 14
};

char *__nos_return_inf(char *const buf, bool neg)
{
    char *ptr = buf;
    *ptr++ = neg ? '-' : '+';
    strcpy(ptr, "inf");
    return buf + strlen(buf);
}

char *__nos_return_nan(char *const buf)
{
    char *ptr = buf;
    strcpy(ptr, "nan");
    return buf + strlen(buf);
}

char *__nos_ldtoa(long double f, char *const buf, int8_t precision)
{
    char *ptr = buf;
    char *p = ptr;
    char *p1 = nullptr;
    char c = 0;
    int64_t intPart = 0;

    if (std::isinf(f))
        return __nos_return_inf(buf, f < 0);

    if (std::isnan(f))
        return __nos_return_nan(buf);

    // check precision bounds
    if (precision > MAX_PRECISION)
        precision = MAX_PRECISION;

    // sign stuff
    if (f < 0)
    {
        f = -f;
        *ptr++ = '-';
    }

    if (precision < 0) // negative precision == automatic precision guess
    {
        if (f < 1.0)
            precision = 6;
        else if (f < 10.0)
            precision = 5;
        else if (f < 100.0)
            precision = 4;
        else if (f < 1000.0)
            precision = 3;
        else if (f < 10000.0)
            precision = 2;
        else if (f < 100000.0)
            precision = 1;
        else
            precision = 0;
    }

    // round value according the precision
    if (precision)
        f += rounders[precision];

    // integer part...
    intPart = (int64_t)f;
    f -= intPart;

    if (!intPart)
        *ptr++ = '0';
    else
    {
        // save start pointer
        p = ptr;

        // convert (reverse order)
        while (intPart)
        {
            *p++ = '0' + intPart % 10;
            intPart /= 10;
        }

        // save end pos
        p1 = p;

        // reverse result
        while (p > ptr)
        {
            c = *--p;
            *p = *ptr;
            *ptr++ = c;
        }

        // restore end pos
        ptr = p1;
    }

    // decimal part
    if (precision)
    {
        // place decimal point
        *ptr++ = '.';

        // convert
        while (precision--)
        {
            f *= 10.0;
            c = (char)f;
            *ptr++ = '0' + c;
            f -= c;
        }
    }

    // terminating zero
    *ptr = 0;

    return ptr;
}

char *__nos_ftoa(float f, char *const buf, int8_t precision)
{
    if (std::isinf(f))
        return __nos_return_inf(buf, f < 0);

    if (std::isnan(f))
        return __nos_return_nan(buf);

    return __nos_ldtoa(f, buf, precision);
}

char *__nos_dtoa(double f, char *const buf, int8_t precision)
{
    if (std::isinf(f))
        return __nos_return_inf(buf, f < 0);

    if (std::isnan(f))
        return __nos_return_nan(buf);

    return __nos_ldtoa(f, buf, precision);
}

char *__nos_itoa(int64_t num, char *const buf, uint8_t base)
{
    char *p = buf;
    char *p1, *p2;
    uint64_t ud = 0;

    *buf = '\0'; /* initialize buffer. In the case of an error, this will
                    already be in the buffer, indicating that the result is
                    invalid (NULL). */
    p1 = buf;    /* start of buffer */

    // check base
    if (base < 2 || base > 36)
    {
        return buf;
    }

    /* If num < 0, put `-' in the head.  */
    if (num < 0)
    {
        *(p++) = '-';
        p1++;
        ud = -num;
    }
    else
    {
        ud = num;
    }

    /* Divide ud by base until ud == 0.  */
    int16_t remainder = 0;
    do
    {
        remainder = ud % base;
        *(p++) = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
    } while (ud /= base);

    /* Terminate buf.  */
    *p = '\0';

    /* Reverse buffer.  */
    p2 = p - 1; /* end of buffer */
    char tmp;
    while (p1 < p2)
    {
        tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }
    return p;
}

char *__nos_utoa(uint64_t num, char *const buf, uint8_t base)
{

    char *p = buf;
    char *p1, *p2;
    uint64_t ud = 0;

    *buf = '\0'; /* initialize buffer. In the case of an error, this will
                    already be in the buffer, indicating that the result is
                    invalid (NULL). */
    p1 = buf;    /* start of buffer */

    // check base
    if (base < 2 || base > 36)
    {
        return buf;
    }

    ud = num;

    /* Divide ud by base until ud == 0.  */
    int16_t remainder = 0;
    do
    {
        remainder = ud % base;
        *(p++) = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
    } while (ud /= base);

    /* Terminate buf.  */
    *p = '\0';

    /* Reverse buffer.  */
    p2 = p - 1; /* end of buffer */
    char tmp;
    while (p1 < p2)
    {
        tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }
    return p;
}
