/**
 * @file
 * @brief
 *
 * @date 16.11.12
 * @author Anton Bondarev
 * @author Anton Kozlov
 * 		- asm/limits
 */

#ifndef LIMITS_H_
#define LIMITS_H_

#include <asm/limits.h>

#ifndef CHAR_BIT
#define CHAR_BIT 	8
#endif /* CHAR_BIT */

#ifndef CHAR_MIN
#define CHAR_MIN 	SCHAR_MIN
#endif /* CHAR_MIN */

#ifndef CHAR_MAX
#define CHAR_MAX 	SCHAR_MAX
#endif /* CHAR_MAX */

#ifndef SCHAR_MIN
#define SCHAR_MIN 	-127
#endif /* SCHAR_MIN */

#ifndef SCHAR_MAX
#define SCHAR_MAX 	128
#endif /* SCHAR_MAX */

#ifndef UCHAR_MAX
#define UCHAR_MAX 	255
#endif /* UCHAR_MAX */


#ifndef WORD_BIT
#define WORD_BIT 	32
#endif /* WORD_BIT */

#ifndef INT_MIN
#define INT_MIN		(-INT_MAX - 1)
#endif /* INT_MIN */

#ifndef INT_MAX
#define INT_MAX		2147483647
#endif /* INT_MAX */

#ifndef UINT_MAX
#define UINT_MAX	4294967295U
#endif /* UINT_MAX */


#ifndef SHRT_MIN
#define SHRT_MIN	(-32768)
#endif /* SHRT_MIN */

#ifndef SHRT_MAX
#define SHRT_MAX	32767
#endif /* SHRT_MAX */

#ifndef USHRT_MAX
#define USHRT_MAX	65535
#endif /* USHRT_MAX */


#ifndef LONG_BIT
#define LONG_BIT	32
#endif /* LONG_BIT */

#ifndef LONG_MIN
#define LONG_MIN	(-LONG_MAX - 1)
#endif /* LONG_MIN */

#ifndef LONG_MAX
#define LONG_MAX	2147483647L
#endif /* LONG_MAX */

#ifndef ULONG_MAX
#define ULONG_MAX	4294967295UL
#endif /* ULONG_MAX */


#ifndef LLONG_MIN
#define LLONG_MIN	(-LLONG_MAX - 1)
#endif /* LLONG_MIN */

#ifndef LLONG_MAX
#define LLONG_MAX	9223372036854775807LL
#endif /* LLONG_MAX */

#ifndef LONG_DIG
#define LONG_DIG	10
#endif /* LONG_DIG */

#ifndef INT_DIG
#define INT_DIG		10
#endif /* INT_DIG */

#ifndef LLONG_DIG
#define LLONG_DIG	19
#endif /* LLONG_DIG */

#ifndef LONG_MAX_DIG
#define LONG_MAX_DIG    19
#endif /* LONG_MAX_DIG */

#ifndef INT_MAX_EXP
#define INT_MAX_EXP	32
#endif /* INT_MAX_EXP */

#ifndef INT_RADIX
#define INT_RADIX	10
#endif /* INT_RADIX */

#ifndef LLONG_RADIX
#define LLONG_RADIX	10
#endif /* LLONG_RADIX */

#ifndef LONG_RADIX
#define LONG_RADIX	10
#endif /* LONG_RADIX */

#ifndef DBL_RADIX 
#define DBL_RADIX	2
#endif /* DBL_RADIX */

#ifndef LDBL_RADIX
#define LDBL_RADIX	2
#endif /* LDBL_RADIX */


/**
 * Pathname Variable Values
 */

/* Maximum number of bytes in a filename (not including \0) */
#define NAME_MAX 32

/* Maximum number of bytes in a pathname, including \0 character */
#define PATH_MAX 256

/*
Maximum number of bytes in a character, for any supported locale.
    Minimum Acceptable Value: 1
*/
#define MB_LEN_MAX 1

//TODO life is pain
#include <ctype.h>

#endif /* LIMITS_H_ */
