#ifndef GENOS_KERNEL_CSECTION_H
#define GENOS_KERNEL_CSECTION_H

#include <hal/ipl.h>

//ipl_t critical_section_enter();
//void critical_section_leave(ipl_t);

void atomic_section_enter();
void atomic_section_leave();

#endif