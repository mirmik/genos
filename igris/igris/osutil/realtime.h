#ifndef IGRIS_OSUTIL_REALTIME_H
#define IGRIS_OSUTIL_REALTIME_H

#include <igris/compiler.h>

__BEGIN_DECLS

int thread_set_realtime_priority(int id);
int this_thread_set_realtime_priority();

__END_DECLS

#endif
