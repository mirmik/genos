#ifndef STM32_RCC_FUNCTONS_H
#define STM32_RCC_FUNCTONS_H

#include <sys/cdefs.h>
#include <stdint.h>

//#include <asm/core/rcc.h>

struct gpio_regs;
struct usart_regs;
struct timer_regs;

#define CLOCKBUS_NO_PLL 0
#define CLOCKBUS_NO_APB1 1
#define CLOCKBUS_NO_APB2 2

extern uint32_t stm32_declared_hsi_freq;
extern uint32_t stm32_declared_hse_freq;

extern uint32_t stm32_declared_clockbus_freq[3];

__BEGIN_DECLS

void rcc_reset();
void rcc_enable_gpio(struct gpio_regs* g);
void rcc_enable_usart(struct usart_regs* u);
void rcc_enable_timer(struct timer_regs* u);
void rcc_enable_syscfg();

#ifdef CHIP_STM32F1XX
void rcc_enable_afio(struct gpio_regs* g);
#endif

__END_DECLS

#endif