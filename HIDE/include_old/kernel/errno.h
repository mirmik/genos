#ifndef KERNELL_ERRNO
#define KERNELL_ERRNO

#include <sys/cdefs.h>

__BEGIN_DECLS

extern int* __errno_location();

__END_DECLS

#endif