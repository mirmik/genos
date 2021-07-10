#ifndef GENOS_TIME_H
#define GENOS_TIME_H

#include <igris/compiler.h>

__BEGIN_DECLS

int64_t system_time();
float systime_frequency();

static inline int64_t millis_to_systime(int64_t ms) { return ms * (systime_frequency() * 0.001); }

__END_DECLS

#endif