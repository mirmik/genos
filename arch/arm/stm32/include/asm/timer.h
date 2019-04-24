#ifndef STM32_ASM_TIMER_H
#define STM32_ASM_TIMER_H

#include <sys/cdefs.h>
#include <periph/map.h>

#include <igris/util/bits.h>

#define STM32_TIMER_DOWNCOUNT 1
#define STM32_TIMER_UPCOUNT 0
//#define STM32_TIMER_CENTERCOUNT 

__BEGIN_DECLS

static inline void stm32_timer_set_prescaler(struct timer_regs * tim, uint16_t prescaler)
{
	tim->PSC = prescaler - 1;
}

static inline void stm32_timer_set_period(struct timer_regs * tim, uint16_t period)
{
	tim->ARR = period - 1;
}

static inline void stm32_timer_set_counter(struct timer_regs * tim, uint16_t counter)
{
	tim->CNT = counter;
}

static inline void stm32_timer_enable(struct timer_regs * tim, uint8_t en)
{
	bits_lvl(tim->CR1, TIM_CR1_CEN, en);
}

static inline void stm32_timer_set_counter_mode(struct timer_regs * tim, uint8_t mode) 
{
	bits_lvl(tim->CR1, TIM_CR1_DIR, mode);
}

static inline void stm32_timer_set_one_pulse_mode(struct timer_regs * tim, uint8_t en) 
{
	bits_lvl(tim->CR1, TIM_CR1_OPM, en);	
}

__END_DECLS

#endif