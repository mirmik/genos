#ifndef DEBUG_PRINT_H
#define DEBUG_PRINT_H

#include <igris/compiler.h>
#include <igris/util/macro.h>
#include <inttypes.h>
#include <stdbool.h>

//#include <gxx/util/macro.h>

/**
    @file
    2012-2018 mirmik
    Библиотека низкоуровневого отладочного вывода.

    Библиотека предназначена для облегчения низкоуровневой отладки.
    Функции не имеют внешних зависимостей (кроме определения типов
    и аппаратнозависимой инициализации и функции вывода символа),
    линкуются по стандарту "С", что позволяет
    вызывать их в том числе с использованием ассемблера.

    Библиотека призвана помочь в создании плацдарма на неосвоенном железе,
    а так же при отладке средств ввода-вывода.

    Для подключения библиотеки необходимо реализовать платформозависимую функцию
    debug_putchar, выводящую один символ char и
    блокирующую процессор на время проведения этой операции.
*/

__BEGIN_DECLS

/// Базовая функция вывода символа.
/// @detail Имплементируется вне библиотеки.
void debug_putchar(char c);
#define dprchar(c) debug_putchar(c)

/// Базовая функция вывода строки указанной длины.
/// @detail Имплементируется вне библиотеки.
void debug_write(const char *c, int i);

/// Internal strlen implementation
static inline int debug_strlen(const char *c)
{
    int i = 0;
    while (c[i] != 0)
        ++i;
    return i;
}

// Print null-terminated string
void debug_print(const char *c);

static inline void debug_print_newline()
{
    debug_putchar('\r');
    debug_putchar('\n');
}
#define dln() debug_print_newline()

static inline void debug_print_tabs(uint8_t n)
{
    while (n--)
        debug_putchar('\t');
}
#define dprtab() debug_putchar('\t');
#define dprtabs(n) debug_print_tabs(n)

// Print null-terminated string with newline
static inline void debug_print_line(const char *c)
{
    debug_print(c);
    debug_print_newline();
}

/// Print pointer as hexdecimal unsigned int (in natural endian)
void debug_printhex_ptr(const void *a);

// Debug print boolean as 'true' or 'false'.
static inline void debug_print_bool(uint8_t b)
{
    debug_print(b ? "true" : "false");
}

// Debug print boolean as '1' or '0'.
static inline void debug_print_bool_num(uint8_t b)
{
    debug_putchar(b ? '1' : '0');
}

/// Hexidecimal integer type`s representation (in natural endian)
void debug_printhex_uint4(uint8_t b);
void debug_printhex_uint8(uint8_t b);
void debug_printhex_uint16(uint16_t a);
void debug_printhex_uint32(uint32_t a);
void debug_printhex_uint64(uint64_t a);
static inline void debug_printhex_int4(int8_t a)
{
    debug_printhex_uint4((uint8_t)a);
}
static inline void debug_printhex_int8(int8_t a)
{
    debug_printhex_uint8((uint8_t)a);
}
static inline void debug_printhex_int16(int16_t a)
{
    debug_printhex_uint16((uint16_t)a);
}
static inline void debug_printhex_int32(int32_t a)
{
    debug_printhex_uint32((uint32_t)a);
}
static inline void debug_printhex_int64(int64_t a)
{
    debug_printhex_uint64((uint64_t)a);
}

/// Binary integer type`s representation (in natural endian)
void debug_printbin_uint4(uint8_t b);
void debug_printbin_uint8(uint8_t b);
void debug_printbin_uint16(uint16_t a);
void debug_printbin_uint32(uint32_t a);
void debug_printbin_uint64(uint64_t a);
#define debug_printbin_int4(a) debug_printbin_uint4(a)
#define debug_printbin_int8(a) debug_printbin_uint8(a)
#define debug_printbin_int16(a) debug_printbin_uint16(a)
#define debug_printbin_int32(a) debug_printbin_uint32(a)
#define debug_printbin_int64(a) debug_printbin_uint64(a)

/// Decimal integer type`s representation
// void debug_printdec_uint8 (uint8_t b);
// void debug_printdec_uint16(uint16_t a);
// void debug_printdec_uint32(uint32_t a);
// void debug_printdec_uint64(uint64_t a);
// void debug_printdec_int8 (int8_t b);
// void debug_printdec_int16(int16_t a);
// void debug_printdec_int32(int32_t a);
// void debug_printdec_int64(int64_t a);

void debug_printdec_unsigned_char(unsigned char n);
void debug_printdec_unsigned_short(unsigned short n);
void debug_printdec_unsigned_int(unsigned int n);
void debug_printdec_unsigned_long(unsigned long n);
void debug_printdec_unsigned_long_long(unsigned long long n);

void debug_printdec_signed_char(signed char n);
void debug_printdec_signed_short(signed short n);
void debug_printdec_signed_int(signed int n);
void debug_printdec_signed_long(signed long n);
void debug_printdec_signed_long_long(signed long long n);

void debug_printhex_char(char n);
void debug_printhex_signed_char(signed char n);
void debug_printhex_signed_short(signed short n);
void debug_printhex_signed_int(signed int n);
void debug_printhex_signed_long(signed long n);
void debug_printhex_signed_long_long(signed long long n);
void debug_printhex_unsigned_char(unsigned char n);
void debug_printhex_unsigned_short(unsigned short n);
void debug_printhex_unsigned_int(unsigned int n);
void debug_printhex_unsigned_long(unsigned long n);
void debug_printhex_unsigned_long_long(unsigned long long n);

/// Float types debug print
void debug_printdec_float_prec(float a, int prec);
void debug_printdec_double_prec(double a, int prec);
static inline void debug_printdec_float(float a)
{
    debug_printdec_float_prec(a, 8);
}
static inline void debug_printdec_double(double a)
{
    debug_printdec_double_prec(a, 8);
}
static inline void debug_printdec_long_double(long double a)
{
    debug_printdec_double_prec(a, 8);
}

void debug_printhex_float(float a);
void debug_printhex_double(double a);

/// Шестнацатеричное представление строки (in machine endian)
void debug_writehex(const void *address, uint16_t size);
void debug_writehex_reversed(const void *address, uint16_t size);

/// Бинарное представление строки (in machine endian)
void debug_writebin(const void *address, uint16_t size);
void debug_writebin_reversed(const void *address, uint16_t size);

/// Вывод дампа памяти в виде размеченной таблицы
void debug_print_dump(const void *address, uint16_t size);
#define dprdump(a, s) debug_print_dump(a, s)

#define dprdec(arg) debug_printdec_int64(arg)

void dprptr(const void *ptr);
void dprptrln(const void *ptr);

__END_DECLS

#ifndef __cplusplus

#define dpr_(X)                                                                \
    _Generic((X),                    \
    const char*:          debug_print,            \
    char*:                debug_print,            \
    unsigned char:        debug_printdec_unsigned_char,         \
    unsigned short:       debug_printdec_unsigned_short,        \
    unsigned int:         debug_printdec_unsigned_int,          \
    unsigned long:        debug_printdec_unsigned_long,         \
    unsigned long long:   debug_printdec_unsigned_long_long,    \
    signed char:          debug_printdec_signed_char,         \
    signed short:         debug_printdec_signed_short,        \
    signed int:           debug_printdec_signed_int,          \
    signed long:          debug_printdec_signed_long,         \
    signed long long:     debug_printdec_signed_long_long,    \
    float:                debug_printdec_float,   \
    double:               debug_printdec_double,  \
    long double:          debug_printdec_long_double,  \
    bool:                 debug_print_bool        \
)(X)

/*
int8_t:             debug_printdec_int8,    \
    int16_t:            debug_printdec_int16,   \
    int32_t:            debug_printdec_int32,   \
    int64_t:            debug_printdec_int64,   \
*/

#define dprhex(X)                                                              \
    _Generic((X), char                                                         \
             : debug_printhex_char, unsigned char                              \
             : debug_printhex_unsigned_char, unsigned short                    \
             : debug_printhex_unsigned_short, unsigned int                     \
             : debug_printhex_unsigned_int, unsigned long                      \
             : debug_printhex_unsigned_long, unsigned long long                \
             : debug_printhex_unsigned_long_long, signed char                  \
             : debug_printhex_signed_char, signed short                        \
             : debug_printhex_signed_short, signed int                         \
             : debug_printhex_signed_int, signed long                          \
             : debug_printhex_signed_long, signed long long                    \
             : debug_printhex_signed_long_long, float                          \
             : debug_printhex_float, double                                    \
             : debug_printhex_double)(X)

#define dprbin(X)                                                              \
    _Generic((X), uint8_t                                                      \
             : debug_printbin_uint8, uint16_t                                  \
             : debug_printbin_uint16, uint32_t                                 \
             : debug_printbin_uint32, uint64_t                                 \
             : debug_printbin_uint64, int8_t                                   \
             : debug_printbin_int8, int16_t                                    \
             : debug_printbin_int16, int32_t                                   \
             : debug_printbin_int32, int64_t                                   \
             : debug_printbin_int64, float                                     \
             : debug_printbin_float, double                                    \
             : debug_printbin_double)(X)

#define dprhexln(X)                                                            \
    do                                                                         \
    {                                                                          \
        dprhex(X);                                                             \
        dln();                                                                 \
    } while (0)
#define dprbinln(X)                                                            \
    do                                                                         \
    {                                                                          \
        dprhex(X);                                                             \
        dln();                                                                 \
    } while (0)

#define dpr_1(X) dpr_(X)
#define dpr_2(X, Y)                                                            \
    do                                                                         \
    {                                                                          \
        dpr_(X);                                                               \
        dprchar(' ');                                                          \
        dpr_(Y);                                                               \
    } while (0)
#define dpr_3(X, Y, Z)                                                         \
    do                                                                         \
    {                                                                          \
        dpr_(X);                                                               \
        dprchar(' ');                                                          \
        dpr_(Y);                                                               \
        dprchar(' ');                                                          \
        dpr_(Z);                                                               \
    } while (0)
#define dpr_4(X, Y, Z, W)                                                      \
    do                                                                         \
    {                                                                          \
        dpr_(X);                                                               \
        dprchar(' ');                                                          \
        dpr_(Y);                                                               \
        dprchar(' ');                                                          \
        dpr_(Z);                                                               \
        dprchar(' ');                                                          \
        dpr_(W);                                                               \
    } while (0)

#define dpr(...) CONCAT2(dpr_, COUNT_ARGS(__VA_ARGS__))(__VA_ARGS__)
#define dprln(...)                                                             \
    do                                                                         \
    {                                                                          \
        dpr(__VA_ARGS__);                                                      \
        dln();                                                                 \
    } while (0)

#else

static inline void dpr() {}
static inline void dpr(char *obj) { debug_print(obj); }
static inline void dpr(const char *obj) { debug_print(obj); }

static inline void dpr(unsigned char obj) { debug_printdec_unsigned_char(obj); }
static inline void dpr(unsigned short obj)
{
    debug_printdec_unsigned_short(obj);
}
static inline void dpr(unsigned int obj) { debug_printdec_unsigned_int(obj); }
static inline void dpr(unsigned long obj) { debug_printdec_unsigned_long(obj); }
static inline void dpr(unsigned long long obj)
{
    debug_printdec_unsigned_long_long(obj);
}

static inline void dpr(signed char obj) { debug_printdec_signed_char(obj); }
static inline void dpr(signed short obj) { debug_printdec_signed_short(obj); }
static inline void dpr(signed int obj) { debug_printdec_signed_int(obj); }
static inline void dpr(signed long obj) { debug_printdec_signed_long(obj); }
static inline void dpr(signed long long obj)
{
    debug_printdec_signed_long_long(obj);
}
/*static inline void dpr(int8_t obj)          { debug_printdec_int8(obj); }
static inline void dpr(int16_t obj)         { debug_printdec_int16(obj); }
static inline void dpr(int32_t obj)         { debug_printdec_int32(obj); }
static inline void dpr(int64_t obj)         { debug_printdec_int64(obj); }
*/
static inline void dpr(double obj) { debug_printdec_double(obj); }
static inline void dpr(long double obj) { debug_printdec_double(obj); }
static inline void dpr(float obj) { debug_printdec_float(obj); }
static inline void dpr(bool obj) { debug_print_bool(obj); }

/*static inline void dprhex(uint8_t obj)      { debug_printhex_uint8(obj); }
static inline void dprhex(uint16_t obj)     { debug_printhex_uint16(obj); }
static inline void dprhex(uint32_t obj)     { debug_printhex_uint32(obj); }
static inline void dprhex(uint64_t obj)     { debug_printhex_uint64(obj); }*/

static inline void dprhex(char obj) { debug_printhex_char(obj); }

static inline void dprhex(unsigned char obj)
{
    debug_printhex_unsigned_char(obj);
}
static inline void dprhex(unsigned short obj)
{
    debug_printhex_unsigned_short(obj);
}
static inline void dprhex(unsigned int obj)
{
    debug_printhex_unsigned_int(obj);
}
static inline void dprhex(unsigned long obj)
{
    debug_printhex_unsigned_long(obj);
}
static inline void dprhex(unsigned long long obj)
{
    debug_printhex_unsigned_long_long(obj);
}

static inline void dprhex(signed char obj) { debug_printhex_signed_char(obj); }
static inline void dprhex(signed short obj)
{
    debug_printhex_signed_short(obj);
}
static inline void dprhex(signed int obj) { debug_printhex_signed_int(obj); }
static inline void dprhex(signed long obj) { debug_printhex_signed_long(obj); }
static inline void dprhex(signed long long obj)
{
    debug_printhex_signed_long_long(obj);
}

static inline void dprhex(double obj) { debug_printhex_double(obj); }
static inline void dprhex(float obj) { debug_printhex_float(obj); }

static inline void dprbin(uint8_t obj) { debug_printbin_uint8(obj); }
static inline void dprbin(uint16_t obj) { debug_printbin_uint16(obj); }
static inline void dprbin(uint32_t obj) { debug_printbin_uint32(obj); }
static inline void dprbin(uint64_t obj) { debug_printbin_uint64(obj); }
static inline void dprbin(int8_t obj) { debug_printbin_int8(obj); }
static inline void dprbin(int16_t obj) { debug_printbin_int16(obj); }
static inline void dprbin(int32_t obj) { debug_printbin_int32(obj); }
static inline void dprbin(int64_t obj) { debug_printbin_int64(obj); }
// static inline void dprbin(double obj)       { debug_printbin_double(obj); }
// static inline void dprbin(float obj)        { debug_printbin_float(obj); }

template <typename T, typename... Tail>
void dpr(const T &obj, const Tail &... tail)
{
    dpr(obj);
    debug_putchar(' ');
    dpr(tail...);
}
template <typename... T> void dprln(const T &... obj)
{
    dpr(obj...);
    dln();
}
template <typename T> void dprhexln(const T &obj)
{
    dprhex(obj);
    dln();
}
template <typename T> void dprbinln(const T &obj)
{
    dprbin(obj);
    dln();
}

#endif

//Тесты для проверки ассемблерной линковки.
void debug_asmlink_test();

uint8_t debug_asmlink_ret8();
uint16_t debug_asmlink_ret16();
uint32_t debug_asmlink_ret32();
uint64_t debug_asmlink_ret64();

void debug_asmlink_args8x1(uint8_t a);
void debug_asmlink_args8x2(uint8_t a, uint8_t b);
void debug_asmlink_args8x3(uint8_t a, uint8_t b, uint8_t c);
void debug_asmlink_args8x4(uint8_t a, uint8_t b, uint8_t c, uint8_t d);

void debug_asmlink_args16x1(uint16_t a);
void debug_asmlink_args16x2(uint16_t a, uint16_t b);
void debug_asmlink_args16x3(uint16_t a, uint16_t b, uint16_t c);
void debug_asmlink_args16x4(uint16_t a, uint16_t b, uint16_t c, uint16_t d);

void debug_asmlink_args32x1(uint32_t a);
void debug_asmlink_args32x2(uint32_t a, uint32_t b);
void debug_asmlink_args32x3(uint32_t a, uint32_t b, uint32_t c);
void debug_asmlink_args32x4(uint32_t a, uint32_t b, uint32_t c, uint32_t d);

void debug_asmlink_args64x1(uint64_t a);
void debug_asmlink_args64x2(uint64_t a, uint64_t b);
void debug_asmlink_args64x3(uint64_t a, uint64_t b, uint64_t c);
void debug_asmlink_args64x4(uint64_t a, uint64_t b, uint64_t c, uint64_t d);

#ifdef __cplusplus

template <typename Buffer> void dpr(const Buffer &obj)
{
    debug_write(obj.data(), obj.size());
}

// void dpr_dump(const void* obj, uint32_t size);
// void dpr_dump_ascii(void* obj, uint32_t size);

template <typename T> void dprptr(const T *obj)
{
    debug_printhex_ptr((const void *)obj);
}

template <typename T> void dprptrln(const T &obj)
{
    dprptr(obj);
    debug_write("\r\n", 2);
}

#endif //__cplusplus

#define DPRINT(X)                                                              \
    do                                                                         \
    {                                                                          \
        dpr(#X);                                                               \
        debug_putchar(' ');                                                    \
        debug_putchar(':');                                                    \
        debug_putchar(' ');                                                    \
        dprln(X);                                                              \
    } while (0);

#define DPRINTHEX(X)                                                           \
    do                                                                         \
    {                                                                          \
        dpr(#X);                                                               \
        debug_putchar(' ');                                                    \
        debug_putchar(':');                                                    \
        debug_putchar(' ');                                                    \
        dprhexln(X);                                                           \
    } while (0);

#define DPRINTPTR(X)                                                           \
    do                                                                         \
    {                                                                          \
        dpr(#X);                                                               \
        debug_putchar(' ');                                                    \
        debug_putchar(':');                                                    \
        debug_putchar(' ');                                                    \
        dprptrln(X);                                                           \
    } while (0);

#define dprhex_noted(S, X)                                                     \
    do                                                                         \
    {                                                                          \
        dpr(S);                                                                \
        dprhex(X);                                                             \
        dln();                                                                 \
    } while (0)

#endif
