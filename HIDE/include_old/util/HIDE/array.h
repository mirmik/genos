#ifndef GENOS_ARRAY_H
#define GENOS_ARRAY_H

#include "util/macro.h"
#include "defines/decltypeof.h"

#define __array_for_each_ptr(ptr, array, size) \
	for ( decltypeof(array) ptr = (array), end = ptr + (size); ptr != end; ++ptr )

#define array_for_each_ptr(ptr, array, size) \
	__array_for_each_ptr(ptr, array, size)


#endif