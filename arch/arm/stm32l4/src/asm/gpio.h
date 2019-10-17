#ifndef STM32L4_GPIO_H
#define STM32L4_GPIO_H

#include <sys/cdefs.h>
#include <stdint.h>
#include <periph/map.h>

__BEGIN_DECLS

void stm32l4_gpio_write(GPIO_TypeDef* g, uint32_t mask, unsigned char level);

uint32_t stm32l4_gpio_read(GPIO_TypeDef* g, uint32_t mask);

void stm32l4_gpio_toggle(GPIO_TypeDef* g, uint32_t mask);

int stm32l4_gpio_mode(GPIO_TypeDef* gpio, unsigned int mask, uint32_t mode);

int stm32l4_gpio_set_alternate(GPIO_TypeDef* g, uint32_t mask, int32_t alternate);

__END_DECLS

#endif