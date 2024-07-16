#ifndef IGRIS_TYPES_EXTENSION_H
#define IGRIS_TYPES_EXTENSION_H

// Плохо работает на разных архитектурах.

#include <stdint.h>

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

/*#ifdef __cplusplus
static_assert(sizeof(float32_t) == 4, "float32_t size error");
#ifndef WITHOUT_FLOAT64
static_assert(sizeof(float64_t) == 8, "float64_t size error");
#endif
#else
#ifndef _MSC_VER
_Static_assert(sizeof(float32_t) == 4, "float32_t size error");
#ifndef WITHOUT_FLOAT64
_Static_assert(sizeof(float64_t) == 8, "float64_t size error");
#endif
#endif
#endif*/

#if defined(_MSC_VER)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#else
#include <asm/types.h>
#endif

#endif
