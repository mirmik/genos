#ifndef UTIL_XTOX_H
#define UTIL_XTOX_H

#include <genos/compiler.h>

/**
 * Convert integer to string.
 * Converts an integer value to a null-terminated string using the specified base and stores the result in the array given by buf parameter.
 * If num < 0, the resulting string is preceded with '-' (regardless of base). No prefix is used to explicitly specify the base.
 * @param num Number to convert.
 * @param buf Character array to store the result. This should be long enough to contain any possible value.
 * @param base Base for the conversion. 1 < base < 37.
 * @return Pointer to buf.
 */

 __BEGIN_DECLS

char *itoa( int num, char *buf, int base );
char *ltoa( long int num, char *buf, int base );
char *lltoa( long int num, char *buf, int base );

char *utoa( unsigned int num, char *buf, int base );
char *ultoa( unsigned long int num, char *buf, int base );

extern double atof(const char *nptr);
extern int atoi(const char *nptr);

extern long atol(const char *nptr);

__END_DECLS

#endif