#ifndef UTIL_NUMBER_CONVERT
#define UTIL_NUMBER_CONVERT

//Набор функций конвертеров в сишним стиле.

#include <igris/compiler.h>
#include <igris/util/types_extension.h>
#include <inttypes.h>
#include <stdlib.h>

__BEGIN_DECLS

uint8_t igris_atou8(const char *buf, uint8_t base, char **end);
uint16_t igris_atou16(const char *buf, uint8_t base, char **end);
uint32_t igris_atou32(const char *buf, uint8_t base, char **end);
uint64_t igris_atou64(const char *buf, uint8_t base, char **end);

int8_t igris_atoi8(const char *buf, uint8_t base, char **end);
int16_t igris_atoi16(const char *buf, uint8_t base, char **end);
int32_t igris_atoi32(const char *buf, uint8_t base, char **end);
int64_t igris_atoi64(const char *buf, uint8_t base, char **end);

float32_t igris_atof32(const char *str, char **pend);
#ifndef WITHOUT_ATOF64
float64_t igris_atof64(const char *str, char **pend);
#endif

char *igris_i8toa(int8_t num, char *buf, uint8_t base);
char *igris_i16toa(int16_t num, char *buf, uint8_t base);
char *igris_i32toa(int32_t num, char *buf, uint8_t base);
char *igris_i64toa(int64_t num, char *buf, uint8_t base);

char *igris_u8toa(uint8_t num, char *buf, uint8_t base);
char *igris_u16toa(uint16_t num, char *buf, uint8_t base);
char *igris_u32toa(uint32_t num, char *buf, uint8_t base);
char *igris_u64toa(uint64_t num, char *buf, uint8_t base);

char *igris_f32toa(float32_t f, char *buf, int8_t precision);
#ifndef WITHOUT_ATOF64
char *igris_f64toa(float64_t f, char *buf, int8_t precision);
char *igris_ftoa(float64_t f, char *buf, int8_t precision);
#else
char *igris_ftoa(float32_t f, char *buf, int8_t precision);
#endif

double igris_strtod(const char *nptr, char **endptr);

__END_DECLS

#endif
