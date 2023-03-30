#ifndef GENOS_POSIX_TYPES_H
#define GENOS_POSIX_TYPES_H

#include <asm/types.h>
#include <stdint.h>

#include <igris/types-generic/size_t.h>
#include <igris/types-generic/ssize_t.h>

typedef int64_t useconds_t;
typedef int64_t pid_t;
typedef int64_t gid_t;
typedef int64_t uid_t;

typedef unsigned int mode_t;

typedef __u32 dev_t;

#include <igris/types-generic/atomic_t.h>
#include <igris/types-generic/clock_t.h>
#include <igris/types-generic/clockid_t.h>
#include <igris/types-generic/id_t.h>
#include <igris/types-generic/limits.h>
#include <igris/types-generic/null.h>
#include <igris/types-generic/nullptr_t.h>
#include <igris/types-generic/off_t.h>
#include <igris/types-generic/ptrdiff_t.h>
#include <igris/types-generic/size_t.h>
#include <igris/types-generic/ssize_t.h>
#include <igris/types-generic/suseconds_t.h>
#include <igris/types-generic/time_t.h>
#include <igris/types-generic/timer_t.h>
#include <igris/types-generic/wchar_t.h>

#endif