#ifndef ASM_STM32_GPIO_H
#define ASM_STM32_GPIO_H

#include <sys/cdefs.h>
#include <periph/map.h>

typedef GPIO_TypeDef gpio_regs_t;

__BEGIN_DECLS

void stm32_gpio_set_output(GPIO_TypeDef * gpio, unsigned int mask);
void stm32_gpio_set_level(GPIO_TypeDef * gpio, unsigned int mask, uint8_t lvl);

void stm32_gpio_write(GPIO_TypeDef * gpio, unsigned int mask, uint8_t lvl);
unsigned int stm32_gpio_read(GPIO_TypeDef * gpio, unsigned int mask);
void stm32_gpio_toggle(GPIO_TypeDef * gpio, unsigned int mask);

__END_DECLS

#endif