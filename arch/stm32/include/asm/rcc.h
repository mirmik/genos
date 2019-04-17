#ifndef STM32_RCC_FUNCTONS_H
#define STM32_RCC_FUNCTONS_H

#include <sys/cdefs.h>

struct gpio_regs;
struct usart_regs;

//#define AHB1_NO 2
//#define AHB2_NO 3
//#define AHPB1_NO 2
//#define AHPB2_NO 3

#define DECLARED_FREQ_HSI_NO 0
#define DECLARED_FREQ_HSE_NO 1
#define DECLARED_FREQ_PLL_NO 2
#define DECLARED_FREQ_APB1_NO 3
#define DECLARED_FREQ_APB2_NO 4

extern uint32_t stm32_declared_freq [5];

__BEGIN_DECLS

void rcc_reset();
void rcc_enable_gpio(struct gpio_regs* g);
void rcc_enable_usart(struct usart_regs* u);

__END_DECLS

#endif