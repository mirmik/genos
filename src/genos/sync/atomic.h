#ifndef GENOS_KERNEL_ATOMIC_SECTION_H
#define GENOS_KERNEL_ATOMIC_SECTION_H

#include <sys/cdefs.h>

__BEGIN_DECLS

void atomic_section_enter();
void atomic_section_leave();

__END_DECLS

#endif