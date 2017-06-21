#ifndef KERNELL_ERRNO
#define KERNELL_ERRNO

#include "compiler.h"

__BEGIN_DECLS

extern int* __errno_location();

__END_DECLS

#endif