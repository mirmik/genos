// Helper translation unit that compiles the AVR-style allocator sources
// under alternative symbol names so they can coexist with libc on the host.
//
// By redefining malloc/free/realloc before including the implementation files
// we obtain lin_malloc_malloc/lin_malloc_free/lin_malloc_realloc symbols that
// unit tests can call without overriding the system allocator.

#define LIN_MALLOC_MALLOC lin_malloc_malloc
#define LIN_MALLOC_FREE lin_malloc_free
#define LIN_MALLOC_REALLOC lin_malloc_realloc

#include "../compat/mem/lin_malloc.cpp"
#include "../compat/mem/lin_realloc.cpp"

#undef LIN_MALLOC_MALLOC
#undef LIN_MALLOC_FREE
#undef LIN_MALLOC_REALLOC
