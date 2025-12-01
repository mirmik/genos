#ifndef IGRIS_JIFFIES_TIME_H
#define IGRIS_JIFFIES_TIME_H

#include <igris/compiler.h>

typedef int64_t jiffies_t;

extern volatile jiffies_t __jiffies;

__BEGIN_DECLS

void system_tick();
jiffies_t jiffies();

__END_DECLS

#endif
