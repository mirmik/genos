#ifndef GENOS_GPIO_H
#define GENOS_GPIO_H

#include "compiler.h"
#include "inttypes.h"
#include "asm/gpio.h"

#define GPIO_MODE_IN_SECTION          0x000000FF
#define GPIO_MODE_OUT_SECTION         0x0000FF00

#define GPIO_MODE_INPUT             	((int32_t)1 << 0)
#define GPIO_MODE_OUTPUT            	((int32_t)1 << 16)

struct gpio_pin_s {
	struct gpio_regs* regs;
	uint8_t offset;
};

__BEGIN_DECLS

int gpio_settings(struct gpio_regs * const g, gpio_mask_t mask, int32_t mode);

void gpio_set_level(struct gpio_regs* const g, gpio_mask_t mask, unsigned char level);
gpio_mask_t gpio_get_level(struct gpio_regs* const g, gpio_mask_t mask);
void gpio_tgl_level(struct gpio_regs* const g, gpio_mask_t mask);


static inline void pin_settings(const struct gpio_pin_s* s, int32_t mode) {
	gpio_settings(s->regs, s->offset, mode);
}

static inline void pin_set_level(const struct gpio_pin_s* s, uint8_t lvl) {
	gpio_set_level(s->regs, s->offset, lvl);
}

static inline gpio_mask_t pin_get_level(const struct gpio_pin_s* s) {
	return gpio_get_level(s->regs, s->offset);
}

static inline void pin_tgl_level(const struct gpio_pin_s* s) {
	gpio_tgl_level(s->regs, s->offset);
}

//void pin_mode(int pin, int32_t mode);
//void pin_set_level(int pin, uint8_t lvl);
//uint8_t pin_get_level(int pin);
//void pin_tgl_level(int pin);

__END_DECLS

#endif