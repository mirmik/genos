#ifndef IGRIS_STD_PORTABLE_H
#define IGRIS_STD_PORTABLE_H

#include <assert.h>
#include <inttypes.h>
#include <math.h>
#include <new>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef __FLOAT32_T_TYPE
typedef float float32_t;
#else
typedef __float32_t float32_t;
#endif

#ifndef WITHOUT_FLOAT64
#ifndef __FLOAT64_T_TYPE
typedef double float64_t;
#else
typedef __float64_t float64_t;
#endif
#endif

namespace igris
{
    using ptrdiff_t = int;
    using size_t = ::size_t;
    using ssize_t = int32_t;
    // template <class T> using initializer_list = igris::initializer_list<T>;
}

static inline uint8_t HIHALF(uint8_t byte)
{
    return (byte >> 4) & 0x0F;
}
static inline uint8_t LOHALF(uint8_t byte)
{
    return byte & 0x0F;
}

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__

#define INT16_HI(arg) *((uint8_t *)&arg + 0)
#define INT16_LO(arg) *((uint8_t *)&arg + 1)

#define INT32_HHI(arg) *((uint8_t *)&arg + 0)
#define INT32_HLO(arg) *((uint8_t *)&arg + 1)
#define INT32_LHI(arg) *((uint8_t *)&arg + 2)
#define INT32_LLO(arg) *((uint8_t *)&arg + 3)

#define INT64_HHHI(arg) *((uint8_t *)&arg + 0)
#define INT64_HHLO(arg) *((uint8_t *)&arg + 1)
#define INT64_HLHI(arg) *((uint8_t *)&arg + 2)
#define INT64_HLLO(arg) *((uint8_t *)&arg + 3)
#define INT64_LHHI(arg) *((uint8_t *)&arg + 4)
#define INT64_LHLO(arg) *((uint8_t *)&arg + 5)
#define INT64_LLHI(arg) *((uint8_t *)&arg + 6)
#define INT64_LLLO(arg) *((uint8_t *)&arg + 7)

#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

#define INT16_HI(arg) *((uint8_t *)&arg + 1)
#define INT16_LO(arg) *((uint8_t *)&arg + 0)

#define INT32_HHI(arg) *((uint8_t *)&arg + 3)
#define INT32_HLO(arg) *((uint8_t *)&arg + 2)
#define INT32_LHI(arg) *((uint8_t *)&arg + 1)
#define INT32_LLO(arg) *((uint8_t *)&arg + 0)

#define INT64_HHHI(arg) *((uint8_t *)&arg + 7)
#define INT64_HHLO(arg) *((uint8_t *)&arg + 6)
#define INT64_HLHI(arg) *((uint8_t *)&arg + 5)
#define INT64_HLLO(arg) *((uint8_t *)&arg + 4)
#define INT64_LHHI(arg) *((uint8_t *)&arg + 3)
#define INT64_LHLO(arg) *((uint8_t *)&arg + 2)
#define INT64_LLHI(arg) *((uint8_t *)&arg + 1)
#define INT64_LLLO(arg) *((uint8_t *)&arg + 0)

#endif

#define UINT16_HI(arg) INT16_HI(arg)
#define UINT16_LO(arg) INT16_LO(arg)

#define UINT32_HHI(arg) INT32_HHI(arg)
#define UINT32_HLO(arg) INT32_HLO(arg)
#define UINT32_LHI(arg) INT32_LHI(arg)
#define UINT32_LLO(arg) INT32_LLO(arg)

#define UINT64_HHHI(arg) INT64_HHHI(arg)
#define UINT64_HHLO(arg) INT64_HHLO(arg)
#define UINT64_HLHI(arg) INT64_HLHI(arg)
#define UINT64_HLLO(arg) INT64_HLLO(arg)
#define UINT64_LHHI(arg) INT64_LHHI(arg)
#define UINT64_LHLO(arg) INT64_LHLO(arg)
#define UINT64_LLHI(arg) INT64_LLHI(arg)
#define UINT64_LLLO(arg) INT64_LLLO(arg)

namespace igris
{
    static inline uint8_t hex2half(char c)
    {
        return (uint8_t)(c <= '9' ? c - '0' : c - 'A' + 10);
    }

    static inline char half2hex(uint8_t n)
    {
        return (char)(n < 10 ? '0' + n : 'A' - 10 + n);
    }

    static inline uint8_t hex2byte(char hi, char lo)
    {
        return (uint8_t)((hex2half(hi) << 4) + hex2half(lo));
    }

    static inline uint8_t hex_to_uint8(const char *hex)
    {
        uint8_t out;

        out = hex2byte(*(hex + 0), *(hex + 1));

        return out;
    }

    static inline uint16_t hex_to_uint16(const char *hex)
    {
        uint16_t out;

        UINT16_HI(out) = hex2byte(*(hex + 0), *(hex + 1));
        UINT16_LO(out) = hex2byte(*(hex + 2), *(hex + 3));

        return out;
    }

    static inline uint32_t hex_to_uint32(const char *hex)
    {
        uint32_t out;

        UINT32_HHI(out) = hex2byte(*(hex + 0), *(hex + 1));
        UINT32_HLO(out) = hex2byte(*(hex + 2), *(hex + 3));
        UINT32_LHI(out) = hex2byte(*(hex + 4), *(hex + 5));
        UINT32_LLO(out) = hex2byte(*(hex + 6), *(hex + 7));

        return out;
    }

    static inline uint64_t hex_to_uint64(const char *hex)
    {
        uint64_t out;

        UINT64_HHHI(out) = hex2byte(*(hex + 0), *(hex + 1));
        UINT64_HHLO(out) = hex2byte(*(hex + 2), *(hex + 3));
        UINT64_HLHI(out) = hex2byte(*(hex + 4), *(hex + 5));
        UINT64_HLLO(out) = hex2byte(*(hex + 6), *(hex + 7));
        UINT64_LHHI(out) = hex2byte(*(hex + 8), *(hex + 9));
        UINT64_LHLO(out) = hex2byte(*(hex + 10), *(hex + 11));
        UINT64_LLHI(out) = hex2byte(*(hex + 12), *(hex + 13));
        UINT64_LLLO(out) = hex2byte(*(hex + 14), *(hex + 15));

        return out;
    }

    static inline void uint8_to_hex(char *hex, uint8_t in)
    {
        *hex++ = half2hex(HIHALF(in));
        *hex++ = half2hex(LOHALF(in));
    }

    static inline void uint16_to_hex(char *hex, uint16_t in)
    {
        *hex++ = half2hex(HIHALF(UINT16_HI(in)));
        *hex++ = half2hex(LOHALF(UINT16_HI(in)));
        *hex++ = half2hex(HIHALF(UINT16_LO(in)));
        *hex++ = half2hex(LOHALF(UINT16_LO(in)));
    }

    static inline void uint32_to_hex(char *hex, uint32_t in)
    {
        *hex++ = half2hex(HIHALF(UINT32_HHI(in)));
        *hex++ = half2hex(LOHALF(UINT32_HHI(in)));
        *hex++ = half2hex(HIHALF(UINT32_HLO(in)));
        *hex++ = half2hex(LOHALF(UINT32_HLO(in)));
        *hex++ = half2hex(HIHALF(UINT32_LHI(in)));
        *hex++ = half2hex(LOHALF(UINT32_LHI(in)));
        *hex++ = half2hex(HIHALF(UINT32_LLO(in)));
        *hex++ = half2hex(LOHALF(UINT32_LLO(in)));
    }

    static inline void uint64_to_hex(char *hex, uint64_t in)
    {
        *hex++ = half2hex(HIHALF(UINT64_HHHI(in)));
        *hex++ = half2hex(LOHALF(UINT64_HHHI(in)));
        *hex++ = half2hex(HIHALF(UINT64_HHLO(in)));
        *hex++ = half2hex(LOHALF(UINT64_HHLO(in)));
        *hex++ = half2hex(HIHALF(UINT64_HLHI(in)));
        *hex++ = half2hex(LOHALF(UINT64_HLHI(in)));
        *hex++ = half2hex(HIHALF(UINT64_HLLO(in)));
        *hex++ = half2hex(LOHALF(UINT64_HLLO(in)));
        *hex++ = half2hex(HIHALF(UINT64_LHHI(in)));
        *hex++ = half2hex(LOHALF(UINT64_LHHI(in)));
        *hex++ = half2hex(HIHALF(UINT64_LHLO(in)));
        *hex++ = half2hex(LOHALF(UINT64_LHLO(in)));
        *hex++ = half2hex(HIHALF(UINT64_LLHI(in)));
        *hex++ = half2hex(LOHALF(UINT64_LLHI(in)));
        *hex++ = half2hex(HIHALF(UINT64_LLLO(in)));
        *hex++ = half2hex(LOHALF(UINT64_LLLO(in)));
    }

    static inline int igris_isxdigit_helper(int c)
    {
        return (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
    }

    static inline int igris_isblank(int c)
    {
        return c == ' ' || c == '\t';
    }
    static inline int igris_isspace(int c)
    {
        return c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\f' ||
               c == '\v';
    }
    static inline int igris_isdigit(int c)
    {
        return c >= '0' && c <= '9';
    }
    static inline int igris_isxdigit(int c)
    {
        return igris_isdigit(c) || igris_isxdigit_helper(c);
    }
    static inline int igris_isupper(int c)
    {
        return c >= 'A' && c <= 'Z';
    }
    static inline int igris_islower(int c)
    {
        return c >= 'a' && c <= 'z';
    }
    static inline int igris_isalpha(int c)
    {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
    }
    static inline int igris_isalnum(int c)
    {
        return igris_isalpha(c) || igris_isdigit(c);
    }

    // TODO
    static inline int igris_isprint(int c)
    {
        return igris_isalpha(c) || igris_isdigit(c) || (c >= ' ' && c <= '~');
    }

    static inline int igris_toupper(int c)
    {
        return igris_islower(c) ? c + ('A' - 'a') : c;
    }
    static inline int igris_tolower(int c)
    {
        return igris_isupper(c) ? c + ('a' - 'A') : c;
    }

    static inline char *igris_i64toa(int64_t num, char *buf, uint8_t base)
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
    static inline char *igris_i32toa(int32_t num, char *buf, uint8_t base)
    {
        return igris_i64toa((int64_t)num, buf, base);
    }
    static inline char *igris_i16toa(int16_t num, char *buf, uint8_t base)
    {
        return igris_i64toa((int64_t)num, buf, base);
    }
    static inline char *igris_i8toa(int8_t num, char *buf, uint8_t base)
    {
        return igris_i64toa((int64_t)num, buf, base);
    }

    static inline char *igris_u64toa(uint64_t num, char *buf, uint8_t base)
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
            *(p++) = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
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

    static inline char *igris_u32toa(uint32_t num, char *buf, uint8_t base)
    {
        return igris_u64toa((uint64_t)num, buf, base);
    }

    static inline char *igris_u16toa(uint16_t num, char *buf, uint8_t base)
    {
        return igris_u64toa((uint64_t)num, buf, base);
    }

    static inline char *igris_u8toa(uint8_t num, char *buf, uint8_t base)
    {
        return igris_u64toa((uint64_t)num, buf, base);
    }

    static inline uint32_t
    igris_atou32(const char *buf, uint8_t base, char **end)
    {
        uint32_t res = 0;

        for (char c = *buf; ((c = *buf)) && igris_isxdigit(c); buf++)
        {
            res = res * base + hex2half(c);
        }

        if (end)
            *end = (char *)buf - 1;

        return res;
    }

    static inline uint64_t
    igris_atou64(const char *buf, uint8_t base, char **end)
    {
        uint64_t res = 0;

        for (char c = *buf; ((c = *buf)) && igris_isxdigit(c); buf++)
        {
            res = res * base + hex2half(c);
        }

        if (end)
            *end = (char *)buf - 1;

        return res;
    }

    static inline uint16_t
    igris_atou16(const char *buf, uint8_t base, char **end)
    {
        return igris_atou32(buf, base, end);
    }

    static inline uint8_t igris_atou8(const char *buf, uint8_t base, char **end)
    {
        return igris_atou32(buf, base, end);
    }

    static inline int32_t
    igris_atoi32(const char *buf, uint8_t base, char **end)
    {
        uint8_t minus;
        int32_t u;

        minus = *buf == '-';
        if (minus)
            ++buf;

        u = igris_atou32(buf, base, end);
        return minus ? -u : u;
    }

    static inline int64_t
    igris_atoi64(const char *buf, uint8_t base, char **end)
    {
        uint8_t minus;
        int64_t u;

        minus = *buf == '-';
        if (minus)
            ++buf;

        u = igris_atou64(buf, base, end);
        return minus ? -u : u;
    }

    static inline int16_t
    igris_atoi16(const char *buf, uint8_t base, char **end)
    {
        return igris_atoi32(buf, base, end);
    }

    static inline int8_t igris_atoi8(const char *buf, uint8_t base, char **end)
    {
        return igris_atoi32(buf, base, end);
    }

#define MAX_PRECISION (10)
    static const double rounders[MAX_PRECISION + 1] = {
        0.5,          // 0
        0.05,         // 1
        0.005,        // 2
        0.0005,       // 3
        0.00005,      // 4
        0.000005,     // 5
        0.0000005,    // 6
        0.00000005,   // 7
        0.000000005,  // 8
        0.0000000005, // 9
        0.00000000005 // 10
    };

    static inline char *igris_f32toa(float32_t f, char *buf, int8_t precision)
    {
        char *ptr = buf;
        char *p = ptr;
        char *p1;
        char c;
        int32_t intPart;

        if (isinf(f))
        {
            *buf++ = f > 0 ? '+' : '-';
            return strcpy(buf, "inf");
            ;
        }

        if (isnan(f))
        {
            return strcpy(buf, "nan");
            ;
        }

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
            f += (float32_t)rounders[precision];

        // integer part...
        intPart = (int32_t)f;
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

        return buf;
    }

    static inline int64_t local_pow(int b, int n)
    {
        int64_t res = 1;
        while (n--)
        {
            res *= b;
        }
        return res;
    }

    static inline float32_t igris_atof32(const char *str, char **pend)
    {
        if (!igris_isdigit(*str) && *str != '-')
        {
            return 0;
        }

        uint8_t minus = *str == '-' ? 1 : 0;
        if (minus)
            str++;

        char *end;
        unsigned int u = igris_atou32(str, 10, &end);

        str = end;
        if (*str == '.')
        {
            int64_t d = igris_atou64(++str, 10, &end);
            if (pend)
                *pend = end;

            float ret =
                (float)u +
                (float)((double)d / (double)local_pow(10, (int)(end - str)));
            return minus ? -ret : ret;
        }

        else
        {
            if (pend)
                *pend = end;
            return minus ? -(float)u : (float)u;
        }
    }

#ifndef WITHOUT_FLOAT64
    static inline char *igris_f64toa(float64_t f, char *buf, int8_t precision)
    {
        return igris_f32toa((float32_t)f, buf, precision);
    }

    static inline float64_t igris_atof64(const char *nptr, char **endptr)
    {
        double val = 0.0;
        int d = 0;
        int sign = 1;

        if (!nptr)
        {
            return 0.0;
        }

        if (*nptr == '+')
        {
            nptr++;
        }
        else if (*nptr == '-')
        {
            nptr++;
            sign = -1;
        }

        while (*nptr >= '0' && *nptr <= '9')
        {
            val = val * 10.0 + (*nptr - '0');
            nptr++;
        }

        if (*nptr == '.')
        {
            nptr++;
            while (*nptr >= '0' && *nptr <= '9')
            {
                val = val * 10.0 + (*nptr - '0');
                nptr++;
                d--;
            }
        }

        if (*nptr == 'E' || *nptr == 'e')
        {
            int e_sign = 1;
            int e_val = 0;

            nptr++;
            if (*nptr == '+')
            {
                nptr++;
            }
            else if (*nptr == '-')
            {
                nptr++;
                sign = -1;
            }

            while ((*nptr >= '0' && *nptr <= '9'))
            {
                e_val = e_val * 10 + (*nptr - '0');
                nptr++;
            }
            d += e_val * e_sign;
        }

        while (d > 0)
        {
            val *= 10.0;
            d--;
        }
        while (d < 0)
        {
            val *= 0.1;
            d++;
        }

        if (endptr)
        {
            *endptr = (char *)nptr;
        }

        return sign * val;
    }

#ifndef WITHOUT_ATOF64
    static inline double igris_strtod(const char *nptr, char **endptr)
    {
        return igris_atof64(nptr, endptr);
    }
    static inline char *igris_ftoa(float64_t f, char *buf, int8_t precision)
    {
        return igris_f64toa(f, buf, precision);
    }
#else
    static inline double igris_strtod(const char *nptr, char **endptr)
    {
        return igris_atof32(nptr, endptr);
    }
    static inline char *igris_ftoa(float64_t f, char *buf, int8_t precision)
    {
        return igris_f32toa(f, buf, precision);
    }
#endif
#endif
    // detail::type_identify
    namespace detail
    {
        template <class T> struct type_identity
        {
            using type = T;
        }; // or use igris::type_identity (since C++20)
    }

    // igris::void_t
    template <class...> using void_t = void;

    // igris::conditional
    template <bool B, class T, class F> struct conditional
    {
        typedef T type;
    };

    template <class T, class F> struct conditional<false, T, F>
    {
        typedef F type;
    };

    template <bool B, class T, class F>
    using conditional_t = typename conditional<B, T, F>::type;

    // igris::enable_if
    template <bool B, class T = void> struct enable_if
    {
    };

    template <class T> struct enable_if<true, T>
    {
        typedef T type;
    };

    template <bool B, class T = void>
    using enable_if_t = typename enable_if<B, T>::type;

    // igris::integral_contant
    template <class T, T v> struct integral_constant
    {
        static constexpr T value = v;
        typedef T value_type;
        typedef integral_constant type;
        constexpr operator value_type() const noexcept
        {
            return value;
        }
        constexpr value_type operator()() const noexcept
        {
            return value;
        }
    };

    using true_type = igris::integral_constant<bool, true>;
    using false_type = igris::integral_constant<bool, false>;

    // igris::is_same
    template <class T, class U> struct is_same : igris::false_type
    {
    };

    template <class T> struct is_same<T, T> : igris::true_type
    {
    };

    template <class T, class U> constexpr bool is_same_v = is_same<T, U>::value;

    // igris::remove_***
    template <class T> struct remove_const
    {
        typedef T type;
    };
    template <class T> struct remove_const<const T>
    {
        typedef T type;
    };

    template <class T> struct remove_volatile
    {
        typedef T type;
    };
    template <class T> struct remove_volatile<volatile T>
    {
        typedef T type;
    };

    template <class T> struct remove_cv
    {
        typedef typename igris::remove_volatile<
            typename igris::remove_const<T>::type>::type type;
    };

    template <class T> using remove_cv_t = typename remove_cv<T>::type;

    template <class T> using remove_const_t = typename remove_const<T>::type;

    template <class T>
    using remove_volatile_t = typename remove_volatile<T>::type;

    template <class T> struct remove_reference
    {
        typedef T type;
    };
    template <class T> struct remove_reference<T &>
    {
        typedef T type;
    };
    template <class T> struct remove_reference<T &&>
    {
        typedef T type;
    };

    template <class T>
    using remove_reference_t = typename remove_reference<T>::type;

    template <class T> struct remove_cvref
    {
        typedef igris::remove_cv_t<igris::remove_reference_t<T>> type;
    };

    template <class T> using remove_cvref_t = typename remove_cvref<T>::type;

    template <class T> struct remove_extent
    {
        typedef T type;
    };

    template <class T> struct remove_extent<T[]>
    {
        typedef T type;
    };

    template <class T, igris::size_t N> struct remove_extent<T[N]>
    {
        typedef T type;
    };

    template <class T> using remove_extent_t = typename remove_extent<T>::type;

    // igris::add_pointer
    namespace detail
    {
        template <class T>
        auto try_add_pointer(int)
            -> type_identity<typename igris::remove_reference<T>::type *>;
        template <class T> auto try_add_pointer(...) -> type_identity<T>;
    } // namespace detail

    template <class T>
    struct add_pointer : decltype(detail::try_add_pointer<T>(0))
    {
    };

    // igris::add_rvalue_reference
    namespace detail
    {
        template <class T>
        auto try_add_lvalue_reference(int) -> type_identity<T &>;
        template <class T>
        auto try_add_lvalue_reference(...) -> type_identity<T>;

        template <class T>
        auto try_add_rvalue_reference(int) -> type_identity<T &&>;
        template <class T>
        auto try_add_rvalue_reference(...) -> type_identity<T>;
    } // namespace detail

    template <class T>
    struct add_lvalue_reference
        : decltype(detail::try_add_lvalue_reference<T>(0))
    {
    };

    template <class T>
    struct add_rvalue_reference
        : decltype(detail::try_add_rvalue_reference<T>(0))
    {
    };

    // Meta programming helper types.
    template <typename...> struct __or_;

    template <> struct __or_<> : public false_type
    {
    };

    template <typename _B1> struct __or_<_B1> : public _B1
    {
    };

    template <typename _B1, typename _B2>
    struct __or_<_B1, _B2> : public conditional<_B1::value, _B1, _B2>::type
    {
    };

    template <typename _B1, typename _B2, typename _B3, typename... _Bn>
    struct __or_<_B1, _B2, _B3, _Bn...>
        : public conditional<_B1::value, _B1, __or_<_B2, _B3, _Bn...>>::type
    {
    };

    template <typename...> struct __and_;

    template <> struct __and_<> : public true_type
    {
    };

    template <typename _B1> struct __and_<_B1> : public _B1
    {
    };

    template <typename _B1, typename _B2>
    struct __and_<_B1, _B2> : public conditional<_B1::value, _B2, _B1>::type
    {
    };

    template <typename _B1, typename _B2, typename _B3, typename... _Bn>
    struct __and_<_B1, _B2, _B3, _Bn...>
        : public conditional<_B1::value, __and_<_B2, _B3, _Bn...>, _B1>::type
    {
    };

    template <typename _Pp>
    struct __not_ : public integral_constant<bool, !_Pp::value>
    {
    };

    template <igris::size_t Len, igris::size_t Align = sizeof(int)>
    struct aligned_storage
    {
        struct type
        {
            alignas(Align) unsigned char data[Len];
        };
    };
}

namespace igris
{

    ///  Marking input iterators.
    struct input_iterator_tag
    {
    };

    ///  Marking output iterators.
    struct output_iterator_tag
    {
    };

    /// Forward iterators support a superset of input iterator operations.
    struct forward_iterator_tag : public input_iterator_tag
    {
    };

    /// Bidirectional iterators support a superset of forward iterator
    /// operations.
    struct bidirectional_iterator_tag : public forward_iterator_tag
    {
    };

    /// Random-access iterators support a superset of bidirectional
    /// iterator operations.
    struct random_access_iterator_tag : public bidirectional_iterator_tag
    {
    };

    /**
     *  @brief  Common %iterator class.
     *
     *  This class does nothing but define nested typedefs.  %Iterator classes
     *  can inherit from this class to save some work.  The typedefs are then
     *  used in specializations and overloading.
     *
     *  In particular, there are no default implementations of requirements
     *  such as @c operator++ and the like.  (How could there be?)
     */
    template <typename Category,
              typename Tp,
              typename Distance = int,
              typename Pointer = Tp *,
              typename Reference = Tp &>
    struct iterator
    {
        /// One of the @link iterator_tags tag types@endlink.
        typedef Category iterator_category;
        /// The type "pointed to" by the iterator.
        typedef Tp value_type;
        /// Distance between iterators is represented as this type.
        typedef Distance difference_type;
        /// This type represents a pointer-to-value_type.
        typedef Pointer pointer;
        /// This type represents a reference-to-value_type.
        typedef Reference reference;
    };

    /**
     *  @brief  Traits class for iterators.
     *
     *  This class does nothing but define nested typedefs.  The general
     *  version simply @a forwards the nested typedefs from the Iterator
     *  argument.  Specialized versions for pointers and pointers-to-const
     *  provide tighter, more correct semantics.
     */
    template <typename Iterator> struct iterator_traits
    {
        typedef typename Iterator::iterator_category iterator_category;
        typedef typename Iterator::value_type value_type;
        typedef typename Iterator::difference_type difference_type;
        typedef typename Iterator::pointer pointer;
        typedef typename Iterator::reference reference;
    };

    /*

    template<typename _Iterator, typename = __void_t<>>
    struct __iterator_traits { };

    template<typename _Iterator>
    struct __iterator_traits<_Iterator,
               __void_t<typename _Iterator::iterator_category,
                        typename _Iterator::value_type,
                        typename _Iterator::difference_type,
                        typename _Iterator::pointer,
                        typename _Iterator::reference>>
    {
        typedef typename _Iterator::iterator_category iterator_category;
        typedef typename _Iterator::value_type        value_type;
        typedef typename _Iterator::difference_type   difference_type;
        typedef typename _Iterator::pointer           pointer;
        typedef typename _Iterator::reference         reference;
    };

    template<typename _Iterator> struct iterator_traits : public
    __iterator_traits<_Iterator> { };

    */

    /// Partial specialization for pointer types.
    template <typename Tp> struct iterator_traits<Tp *>
    {
        typedef random_access_iterator_tag iterator_category;
        typedef Tp value_type;
        typedef ptrdiff_t difference_type;
        typedef Tp *pointer;
        typedef Tp &reference;
    };

    /// Partial specialization for const pointer types.
    template <typename Tp> struct iterator_traits<const Tp *>
    {
        typedef random_access_iterator_tag iterator_category;
        typedef Tp value_type;
        typedef ptrdiff_t difference_type;
        typedef const Tp *pointer;
        typedef const Tp &reference;
    };

    /**
     *  This function is not a part of the C++ standard but is syntactic
     *  sugar for internal library use only.
     */
    template <typename Iter>
    inline typename iterator_traits<Iter>::iterator_category
    __iterator_category(const Iter &)
    {
        return typename iterator_traits<Iter>::iterator_category();
    }

    // If Iterator has a base returns it otherwise _Iterator is returned
    // untouched
    /*template<typename Iterator, bool HasBase>
    struct _Iter_base
    {
        typedef Iterator iterator_type;
        static iterator_type _S_base(Iterator __it) 	{ return __it; }
    };

    template<typename Iterator>
    struct _Iter_base<Iterator, true>
    {
        typedef typename Iterator::iterator_type iterator_type;
        static iterator_type _S_base(Iterator __it)
        { return __it.base(); }
    };*/
} // namespace

namespace igris
{
    template <typename _InputIterator>
    inline typename iterator_traits<_InputIterator>::difference_type __distance(
        _InputIterator __first, _InputIterator __last, input_iterator_tag)
    {
        typename iterator_traits<_InputIterator>::difference_type __n = 0;
        while (__first != __last)
        {
            ++__first;
            ++__n;
        }
        return __n;
    }

    template <typename _RandomAccessIterator>
    inline typename iterator_traits<_RandomAccessIterator>::difference_type
    __distance(_RandomAccessIterator __first,
               _RandomAccessIterator __last,
               random_access_iterator_tag)
    {
        return __last - __first;
    }

    /**
     *  @brief A generalization of pointer arithmetic.
     *  @param  __first  An input iterator.
     *  @param  __last  An input iterator.
     *  @return  The distance between them.
     *
     *  Returns @c n such that __first + n == __last.  This requires
     *  that @p __last must be reachable from @p __first.  Note that @c
     *  n may be negative.
     *
     *  For random access iterators, this uses their @c + and @c - operations
     *  and are constant time.  For other %iterator classes they are linear
     * time.
     */
    template <typename _InputIterator>
    inline typename iterator_traits<_InputIterator>::difference_type
    distance(_InputIterator __first, _InputIterator __last)
    {
        return igris::__distance(
            __first, __last, igris::__iterator_category(__first));
    }

    template <typename _InputIterator, typename _Distance>
    inline void
    __advance(_InputIterator &__i, _Distance __n, input_iterator_tag)
    {
        assert(__n >= 0);
        while (__n--)
            ++__i;
    }

    template <typename _BidirectionalIterator, typename _Distance>
    inline void __advance(_BidirectionalIterator &__i,
                          _Distance __n,
                          bidirectional_iterator_tag)
    {
        if (__n > 0)
            while (__n--)
                ++__i;
        else
            while (__n++)
                --__i;
    }

    template <typename _RandomAccessIterator, typename _Distance>
    inline void __advance(_RandomAccessIterator &__i,
                          _Distance __n,
                          random_access_iterator_tag)
    {
        __i += __n;
    }

    /**
     *  @brief A generalization of pointer arithmetic.
     *  @param  __i  An input iterator.
     *  @param  __n  The @a delta by which to change @p __i.
     *  @return  Nothing.
     *
     *  This increments @p i by @p n.  For bidirectional and random access
     *  iterators, @p __n may be negative, in which case @p __i is decremented.
     *
     *  For random access iterators, this uses their @c + and @c - operations
     *  and are constant time.  For other %iterator classes they are linear
     * time.
     */
    template <typename _InputIterator, typename _Distance>
    inline void advance(_InputIterator &__i, _Distance __n)
    {
        typename iterator_traits<_InputIterator>::difference_type __d = __n;
        igris::__advance(__i, __d, igris::__iterator_category(__i));
    }

    template <typename _ForwardIterator>
    inline _ForwardIterator
    next(_ForwardIterator __x,
         typename iterator_traits<_ForwardIterator>::difference_type __n = 1)
    {
        igris::advance(__x, __n);
        return __x;
    }

    template <typename _BidirectionalIterator>
    inline _BidirectionalIterator
    prev(_BidirectionalIterator __x,
         typename iterator_traits<_BidirectionalIterator>::difference_type __n =
             1)
    {
        igris::advance(__x, -__n);
        return __x;
    }

} // namespace

namespace igris
{
    template <class T>
    constexpr typename igris::remove_reference<T>::type &&move(T &&t)
    {
        return static_cast<typename remove_reference<T>::type &&>(t);
    }

    template <class S>
    constexpr S &&forward(typename remove_reference<S>::type &a) noexcept
    {
        return static_cast<S &&>(a);
    }

    template <class S>
    constexpr S &&forward(typename remove_reference<S>::type &&a) noexcept
    {
        return static_cast<S &&>(a);
    }

    template <typename T> void destructor(T *ptr)
    {
        ptr->~T();
    }

    template <typename T, typename... Args>
    void constructor(T *ptr, Args &&... args)
    {
        new ((void *)ptr) T(igris::forward<Args>(args)...);
    }

    template <typename T> void copy_constructor(T *ptr, const T &other)
    {
        new ((void *)ptr) T(other);
    }

    template <typename T> void move_constructor(T *ptr, T &&other)
    {
        new ((void *)ptr) T(igris::move(other));
    }

    template <class InputIterator, class EndIterator>
    void array_destructor(InputIterator first, EndIterator last)
    {
        while (first != last)
        {
            igris::destructor(&*first);
            ++first;
        }
    }

    template <class InputIterator, class EndIterator, typename... Args>
    void array_constructor(InputIterator first, EndIterator last, Args... args)
    {
        while (first != last)
        {
            igris::constructor(&*first, args...);
            ++first;
        }
    }
}

namespace igris
{
    template <class BidirectionalIterator1, class BidirectionalIterator2>
    BidirectionalIterator2 move_backward(BidirectionalIterator1 first,
                                         BidirectionalIterator1 last,
                                         BidirectionalIterator2 result)
    {
        while (first != last)
        {
            --result;
            --last;
            *result = igris::move(*last);
        }
        return result;
    }

    template <class InputIterator, class InputIterator2, class OutputIterator>
    OutputIterator
    copy(InputIterator first, InputIterator2 last, OutputIterator result)
    {
        while (first != last)
        {
            *result = *first;
            ++first;
            ++result;
        }
        return result;
    }

}

namespace igris
{
    template <typename T> class allocator
    {
    public:
        using value_type = T;
        using pointer = T *;
        using const_pointer = const T *;
        using reference = T &;

        T *allocate(size_t n)
        {
            return (T *)malloc(n * sizeof(T));
        }

        void deallocate(T *ptr, size_t)
        {
            free(ptr);
        }
    };

    template <typename T, class Allocator = igris::allocator<T>> class vector
    {
    public:
        using iterator = T *;
        using const_iterator = const T *;
        using value_type = T;
        using reference = T &;
        using const_reference = const T &;
        using size_type = ::size_t;
        using difference_type = int;
        using reverse_iterator = T *;
        using const_reverse_iterator = const T *;

    private:
        // resources
        T *m_data = nullptr;
        size_t m_capacity = 0;
        size_t m_size = 0;

        // generated
        Allocator m_alloc = {};

    public:
        T *data()
        {
            return m_data;
        }
        const T *data() const
        {
            return m_data;
        }
        size_t size() const
        {
            return m_size;
        }
        size_t capacity() const
        {
            return m_capacity;
        }

        vector(const Allocator &alloc = Allocator()) : m_alloc(alloc) {}

        // vector(const igris::initializer_list<T> &initializers)
        // {
        //     reserve(initializers.size());
        //     for (auto &a : initializers)
        //         push_back(a);
        // }

        // vector(igris::initializer_list<T> &&initializers)
        // {
        //     reserve(initializers.size());
        //     for (auto &a : initializers)
        //         push_back(a);
        // }

        vector(const vector &other) : m_size(other.m_size)
        {
            m_size = other.m_size;
            if (other.m_size == 0)
            {
                return;
            }

            m_data = m_alloc.allocate(m_size);
            m_capacity = m_size;
            for (auto ip = other.m_data, op = m_data;
                 ip != other.m_data + other.m_size;
                 ip++, op++)
            {
                igris::constructor(op, *ip);
            }
        }

        template <class I, class O> vector(I first, O last)
        {
            if (first == last)
                return;

            reserve(igris::distance(first, last));
            for (; first != last; first++)
            {
                push_back(*first);
            }
        }

        vector(vector &&other)
        {
            m_data = other.m_data;
            m_size = other.m_size;
            m_capacity = other.m_capacity;
            other.m_data = nullptr;
            other.m_capacity = 0;
            other.m_size = 0;
        }

        vector &operator=(const vector &other)
        {
            if (this == &other)
                return *this;

            invalidate();

            m_size = other.m_size;
            m_data = m_alloc.allocate(m_size);
            m_capacity = m_size;
            for (auto ip = other.m_data, op = m_data;
                 ip != other.m_data + other.m_size;
                 ip++, op++)
            {
                igris::constructor(op, *ip);
            }

            return *this;
        }

        vector &operator=(vector &&other)
        {
            if (this == &other)
                return *this;

            invalidate();

            m_data = other.m_data;
            m_capacity = other.m_capacity;
            m_size = other.m_size;
            other.m_data = nullptr;
            other.m_capacity = 0;
            other.m_size = 0;
            return *this;
        }

        vector(size_t sz) : m_data(nullptr), m_capacity(0), m_size(0)
        {
            resize(sz);
        }

        vector(iterator a, const iterator b)
            : m_data(nullptr), m_capacity(0), m_size(0)
        {
            while (a != b)
            {
                push_back(*a++);
            }
        }

        ~vector()
        {
            invalidate();
        }

        T &front()
        {
            return m_data[0];
        }

        T &back()
        {
            return m_data[m_size - 1];
        }

        void invalidate()
        {
            if (m_data)
            {
                igris::array_destructor(begin(), end());
                m_alloc.deallocate(m_data, m_capacity);
            }
            m_data = nullptr;
            m_size = 0;
            m_capacity = 0;
        }

        unsigned char reserve(size_t sz)
        {
            if (sz > m_capacity)
                return changeBuffer(sz);
            return 1;
        }

        void clear()
        {
            for (unsigned int i = 0; i < m_size; ++i)
            {
                igris::destructor(m_data + i);
            }
            m_size = 0;
        }

        iterator begin()
        {
            return m_data;
        }
        iterator end()
        {
            return m_data + m_size;
        }
        iterator rbegin()
        {
            return m_data + m_size - 1;
        }
        const_iterator rend()
        {
            return m_data - 1;
        }

        const_iterator begin() const
        {
            return m_data;
        }
        const_iterator end() const
        {
            return m_data + m_size;
        }
        const_iterator rbegin() const
        {
            return m_data + m_size - 1;
        }
        const_iterator rend() const
        {
            return m_data - 1;
        }

        template <typename... Args> void emplace_back(Args &&... args)
        {
            reserve(m_size + 1);
            igris::constructor(m_data + m_size, igris::forward<Args>(args)...);
            m_size++;
        }

        void push_back(const T &ref)
        {
            reserve(m_size + 1);
            igris::constructor(m_data + m_size, ref);
            m_size++;
        }

        void pop_back()
        {
            igris::destructor(m_data + m_size - 1);
            m_size--;
        }

        bool empty() const
        {
            return m_size == 0;
        }

        bool operator==(const vector &oth) const
        {
            if (size() != oth.size())
                return false;

            auto it = begin();
            auto eit = end();
            auto bit = oth.begin();

            for (; it != eit; ++it, ++bit)
            {
                if (*it != *bit)
                    return false;
            }

            return true;
        }

        bool operator!=(const vector &oth) const
        {
            return !operator==(oth);
        }

        template <typename... Args>
        iterator emplace(const_iterator pos, Args &&... args)
        {
            // TODO insert optimization
            size_t _pos = pos - m_data;

            reserve(m_size + 1);
            m_size++;

            iterator first = m_data + _pos;
            iterator last = igris::prev((iterator)end());
            igris::move_backward(first, last, end());
            new (first) T(igris::forward<Args>(args)...);

            return first;
        }

        iterator insert(const_iterator pos, const T &value)
        {
            // TODO insert optimization
            size_t _pos = pos - m_data;

            reserve(m_size + 1);
            m_size++;

            iterator first = m_data + _pos;
            iterator last = igris::prev((iterator)end());
            igris::move_backward(first, last, (iterator)end());
            *first = value;

            return first;
        }

        iterator insert(iterator pos, const_iterator first, const_iterator last)
        {
            size_t _pos = pos - m_data;
            size_t _first = first - m_data;
            size_t _last = last - m_data;

            size_t sz = _last - _first;
            reserve(m_size + sz);
            m_size += sz;

            iterator first_it = m_data + _pos;
            iterator last_it = igris::prev((iterator)end(), sz);
            igris::move_backward(first_it, last_it, (iterator)end());
            igris::copy(m_data + _first, m_data + _last, first_it);

            return first_it;
        }

        iterator insert(int pos, const T &value)
        {
            return insert(m_data + pos, value);
        }

        /*iterator insert_sorted(T const &item)
        {
            return insert(igris::upper_bound(begin(), (iterator)end(), item),
                          item);
        }*/

        void resize(size_t n)
        {
            reserve(n);
            size_t oldsize = m_size;
            if (n > oldsize)
            {
                for (size_t i = oldsize; i < n; ++i)
                {
                    igris::constructor(m_data + i);
                }
            }
            else
            {
                for (size_t i = n; i < oldsize; ++i)
                {
                    igris::destructor(m_data + i);
                }
            }
            m_size = n;
        }

        void erase(iterator newend)
        {
            m_size = newend - m_data;
        }

        void erase(iterator first, iterator last)
        {
            size_t sz = last - first;
            for (size_t i = 0; i < sz; ++i)
            {
                igris::destructor(first + i);
            }
            igris::move(last, end(), first);
            m_size -= sz;
        }

        // T &at(size_t num)
        // {
        //     if (num >= m_size)
        //         throw igris::out_of_range("vector::at");
        //     return m_data[num];
        // }

        T &operator[](size_t num)
        {
            return m_data[num];
        }

        const T &operator[](size_t num) const
        {
            return m_data[num];
        }

        // const T &at(size_t num) const
        // {
        //     if (num >= m_size)
        //         throw igris::out_of_range("vector::at");
        //     return m_data[num];
        // }

        const T &back() const
        {
            return m_data[m_size - 1];
        }

        const T &front() const
        {
            return m_data[0];
        }

        // bool operator<(const vector &oth) const
        // {
        //     return igris::lexicographical_compare(
        //         begin(), end(), oth.begin(), oth.end());
        // }

    protected:
        unsigned char changeBuffer(size_t sz)
        {
            size_t oldcapacity = m_capacity;
            assert(sz > oldcapacity);
            assert(sz > 0);

            auto newbuf = m_alloc.allocate(sz);
            assert((uintptr_t)newbuf % sizeof(uintptr_t) == 0);

            m_capacity = sz;
            if (m_data == nullptr)
            {
                m_data = newbuf;
                return 1;
            }

            auto ie = end();

            for (auto ip = begin(), op = newbuf; ip != ie; op++, ip++)
            {
                igris::move_constructor(op, igris::move(*ip));
            }

            igris::array_destructor(begin(), end());
            auto oldbuf = m_data;
            m_data = newbuf;
            m_alloc.deallocate(oldbuf, oldcapacity);
            return 1;
        }
    };

    template <class CharT> class char_traits;

    template <class CharT = char,
              class Traits = char_traits<CharT>,
              class Allocator = igris::allocator<char>>
    class basic_string
    {

        CharT *m_data = nullptr;
        size_t m_capacity = 0;
        size_t m_size = 0;

        Allocator m_alloc = {};

    public:
        using size_type = ::size_t;
        constexpr static size_type npos = -1;

    public:
        const char *data() const
        {
            return m_data;
        }
        size_t capacity() const
        {
            return m_capacity;
        }
        size_t size() const
        {
            return m_size;
        }
        size_t length() const
        {
            return m_size;
        }

        bool empty() const
        {
            return m_size == 0;
        }

        basic_string() = default;

        basic_string(const basic_string &other) : basic_string()
        {
            *this = other;
        }

        basic_string(basic_string &&other) : basic_string()
        {
            move(other);
        }

        basic_string(const char *str) : basic_string()
        {
            if (str)
                copy(str, strlen(str));
        }

        basic_string(const char *str, size_t sz) : basic_string()
        {
            if (str && sz)
                copy(str, sz);
        }

        ~basic_string()
        {
            if (m_data)
                m_alloc.deallocate(m_data, m_capacity);
        };

        basic_string &copy(const char *cstr, size_t length)
        {
            if (!reserve(length))
            {
                invalidate();
                return *this;
            }
            m_size = length;
            memcpy(m_data, cstr, length);
            return *this;
        }

        void move(basic_string &rhs)
        {
            if (m_data)
                m_alloc.deallocate(m_data, m_capacity);
            m_data = rhs.m_data;
            m_capacity = rhs.m_capacity;
            m_size = rhs.m_size;
            m_alloc = rhs.m_alloc;
            rhs.m_data = nullptr;
            rhs.m_capacity = 0;
            rhs.m_size = 0;
        }

        void invalidate(void)
        {
            if (m_data)
                m_alloc.deallocate(m_data, m_capacity);
            m_data = nullptr;
            m_capacity = m_size = 0;
        }

        basic_string &operator=(const basic_string &rhs)
        {
            if (this == &rhs)
                return *this;

            if (rhs.m_data)
                copy(rhs.m_data, rhs.m_size);
            else
                invalidate();

            return *this;
        }

        basic_string &operator=(basic_string &&rval)
        {
            if (this != &rval)
                move(rval);
            return *this;
        }

        basic_string &operator=(const char *str)
        {
            copy(str, strlen(str));
            return *this;
        }

        bool operator!=(const char *str) const
        {
            auto othlen = strlen(str);
            auto curlen = size();
            if (othlen != curlen)
                return true;
            return memcmp(m_data, str, othlen) != 0;
        }

        bool operator==(const char *str) const
        {
            auto othlen = strlen(str);
            auto curlen = size();
            if (othlen != curlen)
                return false;
            return memcmp(m_data, str, othlen) == 0;
        }

        bool operator!=(const basic_string &str) const
        {
            auto othlen = str.size();
            auto curlen = size();
            if (othlen != curlen)
                return true;
            return memcmp(m_data, str.m_data, othlen) != 0;
        }

        bool operator==(const basic_string &str) const
        {
            auto othlen = str.size();
            auto curlen = size();
            if (othlen != curlen)
                return false;
            return memcmp(m_data, str.m_data, othlen) == 0;
        }

        char &operator[](size_t i)
        {
            return m_data[i];
        }
        const char &operator[](size_t i) const
        {
            return m_data[i];
        }

        const char *c_str() const
        {
            basic_string *self = (basic_string *)this;
            self->reserve(m_size + 1);
            *(self->m_data + self->m_size) = 0;
            return self->begin();
        };

        char *begin()
        {
            return m_data;
        }
        const char *end()
        {
            return m_data + m_size;
        }
        char *rbegin()
        {
            return m_data + m_size - 1;
        }
        const char *rend()
        {
            return m_data - 1;
        }

        const char *begin() const
        {
            return m_data;
        }
        const char *end() const
        {
            return m_data + m_size;
        }
        const char *rbegin() const
        {
            return m_data + m_size - 1;
        }
        const char *rend() const
        {
            return m_data - 1;
        }

        unsigned char reserve(size_t sz)
        {
            if (m_data && m_capacity >= sz)
                return 1;
            if (changeBuffer(sz))
            {
                return 1;
            }
            return 0;
        }

        size_type find(const char *str, size_t pos = 0) const
        {
            if (pos >= m_size)
                return -1;
            auto len = strlen(str);
            if (len == 0)
                return -1;
            if (len > m_size)
                return -1;
            for (size_t i = pos; i < m_size - len; i++)
            {
                if (memcmp(m_data + i, str, len) == 0)
                    return i;
            }
            return -1;
        }

        unsigned char changeBuffer(size_t maxStrLen)
        {
            size_t oldcap = m_capacity;
            char *newbuf = (char *)m_alloc.allocate(maxStrLen);
            char *oldbuf = m_data;
            memcpy(newbuf, oldbuf, m_size);
            if (newbuf)
            {
                m_data = newbuf;
                m_capacity = maxStrLen;
                m_alloc.deallocate(oldbuf, oldcap);
                return 1;
            }
            return 0;
        }

        unsigned char append(const char *cstr, size_t length)
        {
            size_t newlen = m_size + length;
            if (cstr == nullptr)
                return 0;
            if (length == 0)
                return 1;
            if (!reserve(newlen))
                return 0;
            memcpy(m_data + m_size, cstr, length);
            m_size = newlen;
            return 1;
        }

        void append(char c)
        {
            append(&c, 1);
        }

        void append(const basic_string &str)
        {
            append(str.data(), str.size());
        }

        void push_back(char c)
        {
            append(&c, 1);
        }

        void pop_back()
        {
            if (m_size > 0)
                m_size--;
        }

        int compare(const char *str) const
        {
            auto len = strlen(str);
            if (len > m_size)
                return 1;
            if (len < m_size)
                return -1;
            return memcmp(m_data, str, len);
        }

        basic_string &operator+=(char c)
        {
            append(c);
            return *this;
        }

        basic_string &operator+=(const char *cstr)
        {
            append(cstr, strlen(cstr));
            return *this;
        }

        void clear()
        {
            m_size = 0;
        }

        basic_string &operator+=(const basic_string &str)
        {
            append(str.data(), str.size());
            return *this;
        }

        bool operator<(const basic_string &other) const
        {
            int ret = strncmp(data(),
                              other.data(),
                              size() > other.size() ? size() : other.size());
            if (ret == 0)
                return size() < other.size();
            else
                return ret < 0;
        };

        basic_string &shrink()
        {
            changeBuffer(m_size);
            return *this;
        }

        basic_string &shrink_to_print()
        {
            changeBuffer(m_size + 1);
            return *this;
        }

        basic_string &resize(size_t sz)
        {
            reserve(sz);
            assert(sz <= m_capacity);
            m_size = sz;
            return *this;
        }

        basic_string substr(size_t start, size_t len) const
        {
            basic_string ret;
            ret.reserve(len);
            for (size_t i = start; i < start + len; i++)
            {
                ret.push_back(m_data[i]);
            }
            return ret;
        }

        ssize_t find_last_not_of(char c)
        {
            for (auto i = rbegin(); i != rend(); i--)
            {
                if (*i != c)
                    return i - begin();
            }
            return npos;
        }

        igris::vector<basic_string> split(char delim)
        {
            c_str();
            igris::vector<basic_string> outvec;

            char *strt;
            char *ptr = (char *)data();
            char *end = (char *)data() + size();

            while (true)
            {
                while (ptr != end && *ptr == delim)
                    ptr++;

                if (ptr == end)
                    break;

                strt = ptr;

                while (ptr != end && *ptr != delim)
                    ptr++;

                outvec.emplace_back(strt, ptr - strt);
            }

            return outvec;
        }
    };

    using string = basic_string<>;

    static inline int
    stoi(const igris::string &str, igris::size_t *pos = nullptr, int base = 10)
    {
        (void)pos; // TODO
        return igris_atoi32(str.c_str(), base, nullptr);
    }

    static inline long
    stol(const igris::string &str, igris::size_t *pos = nullptr, int base = 10)
    {
        (void)pos; // TODO
        return igris_atoi32(str.c_str(), base, nullptr);
    }

    static inline long long
    stoll(const igris::string &str, igris::size_t *pos = nullptr, int base = 10)
    {
        (void)pos; // TODO
        return igris_atoi32(str.c_str(), base, nullptr);
    }

    static inline double stod(const igris::string &str,
                              igris::size_t *pos = nullptr)
    {
        (void)pos; // TODO
        return igris_atof32(str.c_str(), nullptr);
    }

    static inline bool operator==(const char *str, const string &str2)
    {
        return str2 == str;
    }

    static inline igris::string operator+(const igris::string &lhs,
                                          const igris::string &rhs)
    {
        igris::string res;
        res.reserve(lhs.size() + rhs.size());
        res.append(lhs);
        res.append(rhs);
        return res;
    }

    static inline igris::string operator+(const igris::string &lhs,
                                          const char *rhs)
    {
        igris::string res;
        res.reserve(lhs.size() + strlen(rhs));
        res.append(lhs);
        res.append(rhs);
        return res;
    }

    static inline igris::string operator+(const char *lhs,
                                          const igris::string &rhs)
    {
        igris::string res;
        res.reserve(strlen(lhs) + rhs.size());
        res.append(lhs);
        res.append(rhs);
        return res;
    }

    static inline igris::string to_string(int val)
    {
        char buf[64];
        igris_i64toa(val, buf, 10);
        return buf;
    }

    static inline igris::string to_string(long val)
    {
        char buf[64];
        igris_i64toa(val, buf, 10);
        return buf;
    }

    static inline igris::string to_string(long long val)
    {
        char buf[64];
        igris_i64toa(val, buf, 10);
        return buf;
    }

    static inline igris::string to_string(unsigned val)
    {
        char buf[64];
        igris_i64toa(val, buf, 10);
        return buf;
    }

    static inline igris::string to_string(unsigned long val)
    {
        char buf[64];
        igris_i64toa(val, buf, 10);
        return buf;
    }

    static inline igris::string to_string(unsigned long long val)
    {
        char buf[64];
        igris_i64toa(val, buf, 10);
        return buf;
    }

    static inline igris::string to_string(float val)
    {
        char buf[64];
        igris_ftoa(val, buf, 5);
        return buf;
    }

    static inline igris::string to_string(double val)
    {
        char buf[64];
        igris_ftoa(val, buf, 5);
        return buf;
    }
}

namespace igris
{
    template <class T, igris::size_t N> class static_vector
    {
    public:
        using value_type = T;
        using pointer = T *;
        using reference = T &;
        using iterator = T *;
        using const_iterator = const T *;

    private:
        // Переместить во внешний буффер.
        typename igris::aligned_storage<sizeof(T), alignof(T)>::type _data[N];
        igris::size_t m_size = 0;

    public:
        static_vector()
        {
            memset(_data, 0, sizeof(_data));
        }

        static_vector(const static_vector &other)
        {
            m_size = other.m_size;
            for (igris::size_t pos = 0; pos < m_size; ++pos)
            {
                new (&_data[pos]) T(other[pos]);
            }
        }

        static_vector(static_vector &&other)
        {
            m_size = other.m_size;
            for (igris::size_t pos = 0; pos < m_size; ++pos)
            {
                new (&_data[pos]) T(igris::move(other[pos]));
            }
        }

        static_vector &operator=(const static_vector &other)
        {
            m_size = other.m_size;
            for (igris::size_t pos = 0; pos < m_size; ++pos)
            {
                new (&_data[pos]) T(other[pos]);
            }
            return *this;
        }

        static_vector &operator=(static_vector &&other)
        {
            m_size = other.m_size;
            for (igris::size_t pos = 0; pos < m_size; ++pos)
            {
                new (&_data[pos]) T(igris::move(other[pos]));
            }
            other.m_size = 0;
            return *this;
        }

        /*static_vector(const igris::initializer_list<T> &lst)
        {
            for (auto &obj : lst)
            {
                new (&_data[m_size]) T(obj);
                ++m_size;
            }
        }*/

        // Create an object in aligned storage
        template <typename... Args> void emplace_back(Args &&... args)
        {
            if (m_size >= N)
                return;
            new (&_data[m_size]) T(igris::forward<Args>(args)...);
            ++m_size;
        }

        void push_back(const T &obj)
        {
            if (m_size >= N)
                return;
            new (&_data[m_size]) T(obj);
            ++m_size;
        }

        T &operator[](igris::size_t pos)
        {
            return *reinterpret_cast<T *>(&_data[pos]);
        }

        const T &operator[](igris::size_t pos) const
        {
            return *reinterpret_cast<const T *>(&_data[pos]);
        }

        T *data()
        {
            return reinterpret_cast<T *>(&_data[0]);
        }

        const T *data() const
        {
            return reinterpret_cast<const T *>(&_data[0]);
        }

        igris::size_t room() const
        {
            return N - m_size;
        }
        igris::size_t size() const
        {
            return m_size;
        }

        // Delete objects from aligned storage
        ~static_vector()
        {
            for (igris::size_t pos = 0; pos < m_size; ++pos)
            {
                reinterpret_cast<T *>(&_data[pos])->~T();
            }
            m_size = 0;
        }

        iterator begin()
        {
            return reinterpret_cast<T *>(&_data[0]);
        }

        const_iterator end()
        {
            return reinterpret_cast<T *>(&_data[m_size]);
        }

        const_iterator begin() const
        {
            return reinterpret_cast<const T *>(&_data[0]);
        }

        const_iterator end() const
        {
            return reinterpret_cast<const T *>(&_data[m_size]);
        }

        T &back()
        {
            return *reinterpret_cast<T *>(&_data[m_size - 1]);
        }

        const T &back() const
        {
            return *reinterpret_cast<const T *>(&_data[m_size - 1]);
        }

        T &front()
        {
            return *reinterpret_cast<T *>(&_data[0]);
        }

        const T &front() const
        {
            return *reinterpret_cast<const T *>(&_data[0]);
        }

        void resize(size_t newsize)
        {
            if (newsize >= N)
                newsize = N;

            for (size_t i = m_size; i < newsize; ++i)
            {
                new (&_data[i]) T{};
            }

            m_size = newsize;
        }

        void clear()
        {
            m_size = 0;
        }
    };
}

namespace igris
{
    template <igris::size_t N> class static_string
    {
        using pointer = char *;
        using reference = char &;
        using iterator = char *;
        using const_iterator = const char *;

        // Переместить во внешний буффер.
        mutable char _data[N + 1];
        igris::size_t m_size = 0;

    public:
        static_string() = default;

        static_string(const char *dat)
        {
            m_size = strlen(dat);
            memcpy(_data, dat, m_size);
        }

        static_string(const char *dat, size_t sz)
        {
            m_size = sz;
            memcpy(_data, dat, sz);
        }

        char *data()
        {
            return _data;
        }

        void clear()
        {
            m_size = 0;
        }

        static_string &operator+=(char c)
        {
            push_back(c);
            return *this;
        }

        char &operator[](igris::size_t pos)
        {
            return _data[pos];
        }

        char operator[](igris::size_t pos) const
        {
            return _data[pos];
        }

        igris::size_t room()
        {
            return N - m_size;
        }
        igris::size_t size()
        {
            return m_size;
        }

        // Delete objects from aligned storage
        ~static_string() = default;

        iterator begin()
        {
            return &_data[0];
        }
        const_iterator end()
        {
            return &_data[m_size];
        }

        int find(const char *str, size_t pos = 0) const
        {
            if (pos >= m_size)
                return -1;
            auto len = strlen(str);
            if (len == 0)
                return -1;
            if (len > m_size)
                return -1;
            for (size_t i = pos; i < m_size - len; i++)
            {
                if (memcmp(data() + i, str, len) == 0)
                    return i;
            }
            return -1;
        }

        template <size_t VSize, size_t SSize>
        igris::static_vector<static_string<SSize>, VSize> split(char delim)
        {
            igris::static_vector<static_string<SSize>, VSize> outvec;

            char *strt;
            char *ptr = (char *)_data;
            char *end = (char *)_data + size();

            while (true)
            {
                while (ptr != end && *ptr == delim)
                    ptr++;

                if (ptr == end)
                    break;

                strt = ptr;

                while (ptr != end && *ptr != delim)
                    ptr++;

                outvec.emplace_back(strt, ptr - strt);
            }

            return outvec;
        }

        void push_back(char c)
        {
            if (m_size >= N)
                return;

            _data[m_size++] = c;
        }

        const char *c_str() const
        {
            _data[m_size] = 0;
            return _data;
        }
    };

    template <size_t Sz>
    static inline int stoi(const igris::static_string<Sz> &str,
                           igris::size_t *pos = nullptr,
                           int base = 10)
    {
        (void)pos; // TODO
        return igris_atoi32(str.c_str(), base, nullptr);
    }

    template <size_t Sz>
    static inline long stol(const igris::static_string<Sz> &str,
                            igris::size_t *pos = nullptr,
                            int base = 10)
    {
        (void)pos; // TODO
        return igris_atoi32(str.c_str(), base, nullptr);
    }

    template <size_t Sz>
    static inline long long stoll(const igris::static_string<Sz> &str,
                                  igris::size_t *pos = nullptr,
                                  int base = 10)
    {
        (void)pos; // TODO
        return igris_atoi32(str.c_str(), base, nullptr);
    }

    template <size_t Sz>
    static inline double stod(const igris::static_string<Sz> &str,
                              igris::size_t *pos = nullptr)
    {
        (void)pos; // TODO
        return igris_atof32(str.c_str(), nullptr);
    }

}

#endif
