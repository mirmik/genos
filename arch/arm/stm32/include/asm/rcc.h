#ifndef STM32_RCC_FUNCTONS_H
#define STM32_RCC_FUNCTONS_H

#include <compiler.h>

struct gpio_regs;
struct usart_regs;

__BEGIN_DECLS

void rcc_enable_gpio(struct gpio_regs* g);
void rcc_enable_usart(struct usart_regs* u);

__END_DECLS

#endif