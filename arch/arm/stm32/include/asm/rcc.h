#ifndef STM32_RCC_FUNCTONS_H
#define STM32_RCC_FUNCTONS_H

#include <compiler.h>

struct gpio_regs;

__BEGIN_DECLS

void rcc_enable_gpio(struct gpio_regs* g);

__END_DECLS

#endif