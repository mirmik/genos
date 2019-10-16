#ifndef ARM_DWT_H
#define ARM_DWT_H

#include <sys/cdefs.h>
#include <stdint.h>

__BEGIN_DECLS

void arm_dwt_init();
void arm_dwt_delay_us(uint32_t us);

__END_DECLS

#endif