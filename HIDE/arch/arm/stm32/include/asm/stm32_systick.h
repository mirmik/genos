#ifndef STM32_SYSTICK_H
#define STM32_SYSTICK_H

#include <stdint.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

int stm32_systick_config(uint32_t ticks);

__END_DECLS

#endif