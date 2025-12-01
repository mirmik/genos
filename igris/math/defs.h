#ifndef IGRIS_MATH_DEFS_H
#define IGRIS_MATH_DEFS_H

#include <math.h>

#define __MAX__(a, b) ((a) > (b) ? (a) : (b))
#define __MIN__(a, b) ((a) < (b) ? (a) : (b))
#define __ABS__(a) ((a) < 0 ? (-(a)) : (a))

#ifndef MAX
#define MAX(a, b) __MAX__(a, b)
#endif

#ifndef MIN
#define MIN(a, b) __MIN__(a, b)
#endif

#ifndef ABS
#define ABS(a) __ABS__(a)
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_E
#define M_E 2.71828182845904523536
#endif

#ifndef M_LOG2E
#define M_LOG2E 1.44269504088896340736
#endif

#ifndef M_LOG10E
#define M_LOG10E 0.434294481903251827651
#endif

#ifndef M_LN2
#define M_LN2 0.693147180559945309417
#endif

#ifndef M_LN10
#define M_LN10 2.30258509299404568402
#endif

#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923
#endif

#ifndef M_PI_4
#define M_PI_4 0.785398163397448309616
#endif

#ifndef M_1_PI
#define M_1_PI 0.318309886183790671538
#endif

#ifndef M_2_PI
#define M_2_PI 0.636619772367581343076
#endif

#ifndef M_2_SQRTPI
#define M_2_SQRTPI 1.12837916709551257390
#endif

#ifndef M_SQRT2
#define M_SQRT2 1.41421356237309504880
#endif

#ifndef M_SQRT1_2
#define M_SQRT1_2 0.707106781186547524401
#endif

#endif
