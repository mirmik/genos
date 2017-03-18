#ifndef DRIVERS_STM32_RCC
#define DRIVERS_STM32_RCC

#include <compiler.h>
#include <periph/map.h>

__BEGIN_DECLS

void rcc_enable_gpio(struct gpio_regs* gpio);

__END_DECLS

#endif