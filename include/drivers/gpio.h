#ifndef GENOS_GPIO_H
#define GENOS_GPIO_H

#include "compiler.h"
#include "inttypes.h"
#include "asm/gpio.h"

#define GPIO_MODE_IN_SECTION          0x000000FF
#define GPIO_MODE_OUT_SECTION         0x0000FF00

#define GPIO_MODE_INPUT             	((int32_t)1 << 0)
#define GPIO_MODE_OUTPUT            	((int32_t)1 << 16)

__BEGIN_DECLS

int gpio_settings(struct gpio_regs *g, gpio_mask_t mask, int32_t mode);

void gpio_set_level(struct gpio_regs* g, gpio_mask_t mask, unsigned char level);
gpio_mask_t gpio_get_level(struct gpio_regs* g, gpio_mask_t mask);
void gpio_tgl_level(struct gpio_regs* g, gpio_mask_t mask);

void pin_mode(int pin, int32_t mode);
void pin_set_level(int pin, uint8_t lvl);
uint8_t pin_get_level(int pin);
void pin_tgl_level(int pin);

__END_DECLS

#endif