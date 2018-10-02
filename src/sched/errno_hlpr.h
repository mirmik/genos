#ifndef GENOS_ERRNO_HLPR_H
#define GENOS_ERRNO_HLPR_H

#include <errno.h>

__BEGIN_DECLS

int * set_errno_location(int * new);

__END_DECLS

#endif