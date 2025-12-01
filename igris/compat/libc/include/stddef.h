/**
 * @file
 * @brief C standard library header.
 * @details Defines the macros #NULL and #offsetof as well
 *          as the types #ptrdiff_t and #size_t.
 *
 * @date 09.03.10
 * @author Eldar Abusalimov
 */

#ifndef STDDEF_H_
#define STDDEF_H_

//#if __GNUC__ < 4 || defined(__CDT_PARSER__)
#define offsetof(type, member) ((size_t) & ((type *)0x0)->member)
//#else
//# define offsetof(type, member)
//	__builtin_offsetof(type, member)
//#endif

#include <igris/types-generic/null.h>
#include <igris/types-generic/ptrdiff_t.h>
#include <igris/types-generic/size_t.h>
#include <igris/types-generic/wchar_t.h>

#endif /* STDDEF_H_ */
