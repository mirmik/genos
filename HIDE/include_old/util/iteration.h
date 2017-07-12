#ifndef UTIL_ITERATION_H
#define UTIL_ITERATION_H

#include "util/macro.h"

#define do_times(i) for(int _iteration = 0; _iteration < i; ++_iteration )

#define static_do_once \
if (({ static int __mgrd(v0) = 1; __mgrd(v0)-- == 1 ? 1 : 0; })) 

#endif