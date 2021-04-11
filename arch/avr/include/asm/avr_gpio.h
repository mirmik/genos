#ifndef AVR_GPIO_H
#define AVR_GPIO_H

#include "inttypes.h"

struct gpio_regs {
	volatile uint8_t pin;
	volatile uint8_t ddr;
	volatile uint8_t port;
} __attribute__((packed));

__BEGIN_DECLS

int avr_gpio_set(struct gpio_regs* g, uint16_t mask, unsigned char level);
int avr_gpio_get(struct gpio_regs* g, uint16_t mask);
int avr_gpio_toggle(struct gpio_regs* g, uint16_t mask);
int avr_gpio_setup(struct gpio_regs* g, uint16_t mask, int32_t mode);

__END_DECLS

#endif