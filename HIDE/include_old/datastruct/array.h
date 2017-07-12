#ifndef GENOS_DATASTRUCT_ARRAY_H
#define GENOS_DATASTRUCT_ARRAY_H

#include <genos/defines/decltypeof.h>
#include <util/macro.h>

#define __array_for_each(it,arr,sz,end) for(decltypeof(arr) it = arr, end = arr + sz; it != end; it++)
#define array_for_each(it,arr,sz) __array_for_each(it,arr,sz,MACRO_GUARD(end))

#endif