#ifndef NOS_UTIL_NUMCONVERT_H
#define NOS_UTIL_NUMCONVERT_H

#include <stdint.h>

char *__nos_ftoa(float f, char *buf, int8_t precision);
char *__nos_dtoa(double f, char *buf, int8_t precision);
char *__nos_ldtoa(long double f, char *buf, int8_t precision);
char *__nos_itoa(int64_t num, char *buf, uint8_t base);
char *__nos_utoa(uint64_t num, char *buf, uint8_t base);

#endif
