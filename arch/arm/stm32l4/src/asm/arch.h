#ifndef STM32L4_ARCH_H
#define STM32L4_ARCH_H

#include <sys/cdefs.h>
#include <periph/map.h>

extern int32_t stm32_declared_clockbus_freq[3];

#define CLOCKBUS_NO_PLL 0
#define CLOCKBUS_NO_APB1 0
#define CLOCKBUS_NO_APB2 0

__BEGIN_DECLS

void stm32l4_diag_init(usart_regs_t* regs);

__END_DECLS

#endif