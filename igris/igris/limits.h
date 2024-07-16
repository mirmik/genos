#ifndef IGRIS_LIMITS_H
#define IGRIS_LIMITS_H

#include <limits.h>

#ifndef LLONG_MAX
#define LLONG_MAX ((long long)((unsigned long long)(-1) >> 1))
#endif

#ifndef LLONG_MIN
#define LLONG_MIN ((long long)(((unsigned long long)(-1) >> 1) + 1))
#endif

#ifndef LONG_MAX
#define LONG_MAX ((long)((unsigned long)(-1) >> 1))
#endif

#ifndef LONG_MIN
#define LONG_MIN ((long)(((unsigned long)(-1) >> 1) + 1))
#endif

#ifndef INT_MAX
#define INT_MAX ((int)((unsigned int)(-1) >> 1))
#endif

#ifndef INT_MIN
#define INT_MIN ((int)(((unsigned int)(-1) >> 1) + 1))
#endif

#endif
