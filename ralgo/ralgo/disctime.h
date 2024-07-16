#ifndef RALGO_DISCRETE_TIME_H
#define RALGO_DISCRETE_TIME_H

#include <igris/compiler.h>
#include <stdint.h>

#if __has_include(<asm/heimer_types.h>)
#include <asm/heimer_types.h>
#else

typedef int64_t disctime_t;

__BEGIN_DECLS

disctime_t discrete_time();
float discrete_time_frequency();

__END_DECLS

#endif

#ifdef __cplusplus

namespace ralgo
{
    using ::discrete_time;
    using ::discrete_time_frequency;
}

#endif

#endif
