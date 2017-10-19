#ifndef GENOS_KERNEL_SYSTEM_H
#define GENOS_KERNEL_SYSTEM_H

#include <sys/cdefs.h>

__BEGIN_DECLS

void cxx_invoke_constructors(void);
void __do_global_ctors(void);
void __cxa_pure_virtual();
void end_of_programm(void);

__END_DECLS

#endif 