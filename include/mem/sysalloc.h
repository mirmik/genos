#ifndef GENOS_SYSALLOC_H
#define GENOS_SYSALLOC_H

#include <mem/allocator.h>

#ifdef GENOS_CONFIGURE
#	include "genos_configure.h"
#endif

#ifndef sysalloc
#	define sysalloc(sz) malloc(sz)
#endif

#ifndef sysfree
#	define sysfree(ptr) free(ptr)
#endif

#ifndef sysrealloc
#	define sysrealloc(ptr,sz) realloc(ptr,sz)
#endif

#endif