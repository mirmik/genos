#ifndef _A_TRACE_H
#define _A_TRACE_H

#define NODTRACE 0
#include <igris/dtrace.h>

#define DTRACE_DOEVENT_ENABLE 0

#define DTRACE_DOEVENT() DTRACE_CONDITION(DTRACE_DOEVENT_ENABLE)
#define DTRACE_ARGS_DOEVENT(...)                                               \
    DTRACE_ARGS_CONDITION(DTRACE_DOEVENT_ENABLE, ...)

#endif