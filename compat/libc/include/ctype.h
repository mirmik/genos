#ifndef CTYPE_H_
#define CTYPE_H_

#include <igris/compiler.h>
#include <igris/util/ctype.h>

__BEGIN_DECLS

static inline int isalnum(int c) { return igris_isalnum(c); }
static inline int isalpha(int c) { return igris_isalpha(c); }
static inline int isblank(int c) { return igris_isblank(c); }
//static inline int iscntrl(int c) { return __ismask(c) & (_C); }
static inline int isdigit(int c) { return igris_isdigit(c); }
//static inline int isgraph(int c) { return __ismask(c) & (_P | _U | _L | _D); }
static inline int islower(int c) { return igris_islower(c); }
static inline int isprint(int c) { return igris_isprint(c); }
//static inline int ispunct(int c) { return __ismask(c) & (_P); }
static inline int isspace(int c) { return igris_isspace(c); }
static inline int isupper(int c) { return igris_isupper(c); }
static inline int isxdigit(int c) { return igris_isxdigit(c); }

static inline int tolower(int c) { return igris_tolower(c); }
static inline int toupper(int c) { return igris_toupper(c); }

__END_DECLS

/* Checks whether c is a 7-bit unsigned char value that
 * fits into the ASCII character set. */
#define isascii(c) (((unsigned char)(c))<=0x7f)
#define toascii(c) (((unsigned char)(c))&0x7f)

#endif /* CTYPE_H_ */
