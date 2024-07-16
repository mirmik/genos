#ifndef STM32_ASM_EXTI_H
#define STM32_ASM_EXTI_H

#include <stm32xxxx.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

static inline
void stm32_exti_unmask(uint8_t lineno, uint8_t en) 
{
	bits_lvl(EXTI->IMR, (1<<lineno), en); 
}

static inline
void stm32_exti_rising_trigger_enable(uint8_t lineno, uint8_t en) 
{
	bits_lvl(EXTI->RTSR, (1<<lineno), en); 
}

static inline
void stm32_exti_falling_trigger_enable(uint8_t lineno, uint8_t en) 
{
	bits_lvl(EXTI->FTSR, (1<<lineno), en); 
}

__END_DECLS

#endif