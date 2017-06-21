#ifndef GENOS_KERNEL_CSECTION_H
#define GENOS_KERNEL_CSECTION_H

#include <hal/irq.h>

//ipl_t critical_section_enter();
//void critical_section_leave(ipl_t);

__BEGIN_DECLS
void atomic_section_enter();
void atomic_section_leave();
__END_DECLS

#define critical_section_enter() atomic_section_enter()
#define critical_section_leave() atomic_section_leave()

#endif