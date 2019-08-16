#ifndef STM32_ASM_TIMER_H
#define STM32_ASM_TIMER_H

#include <sys/cdefs.h>
#include <periph/map.h>

#include <igris/util/bits.h>

#define STM32_TIMER_DOWNCOUNT 1
#define STM32_TIMER_UPCOUNT 0
//#define STM32_TIMER_CENTERCOUNT 

__BEGIN_DECLS

// Установить предделитель. Частота тика таймера равна частоте его
// шины деленной на значение предделителя.
static inline void stm32_timer_set_prescaler(struct timer_regs * tim, uint16_t prescaler)
{
	tim->PSC = prescaler - 1;
}

// Установить период счета в тиках таймера. После достижения значения периода
// таймер сбрасывается.
static inline void stm32_timer_set_period(struct timer_regs * tim, uint16_t period)
{
	tim->ARR = period - 1;
}

// Установить текущее значение таймера.
static inline void stm32_timer_set_counter(struct timer_regs * tim, uint16_t counter)
{
	tim->CNT = counter;
}

// Считать значение счетчика.
static inline uint32_t stm32_timer_counter(struct timer_regs * tim)
{
	return tim->CNT;
}

// Включение выключения счета таймера.
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

// Активировать прерывание по переполнению ( бит UIE - update interrupt enable ). 
// Не забудьте настроить разрешение в NVIC.
// Перед выходом из прерывания следует сбросить флаг SR->UIF.                  
static inline
void stm32_timer_ovf_irq_enable(struct timer_regs * tim, uint8_t en) 
{
	bits_lvl(tim->DIER, TIM_DIER_UIE, en);
} 

static inline
void stm32_timer_drop_ovf_flag(struct timer_regs * tim) 
{
	bits_clr(tim->SR, TIM_SR_UIF);
}

__END_DECLS

#endif