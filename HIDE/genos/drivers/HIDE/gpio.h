#ifndef GENOS_GPIO_H
#define GENOS_GPIO_H

#include "inttypes.h"
#include <sys/cdefs.h>

struct gpio_pin_s {
	struct gpio_regs* regs;
	uint8_t offset;
};

#include "asm/gpio.h"

#define GPIO_MODE_IN_SECTION          0x000000FF
#define GPIO_MODE_OUT_SECTION         0x0000FF00

#define GPIO_MODE_INPUT             	((int32_t)1 << 0)
#define GPIO_MODE_OUTPUT            	((int32_t)1 << 16)

__BEGIN_DECLS

int gpio_settings(struct gpio_regs * const g, gpio_mask_t mask, int32_t mode);

void gpio_write(struct gpio_regs* const g, gpio_mask_t mask, unsigned char level);

gpio_mask_t gpio_read(struct gpio_regs* const g, gpio_mask_t mask);

void gpio_toggle(struct gpio_regs* const g, gpio_mask_t mask);


static inline void pin_settings(const struct gpio_pin_s* s, int32_t mode) {
	gpio_settings(s->regs, 1 << s->offset, mode);
}

static inline void pin_set_level(const struct gpio_pin_s* s, uint8_t lvl) {
	gpio_write(s->regs, 1 << s->offset, lvl);
}

static inline gpio_mask_t pin_get_level(const struct gpio_pin_s* s) {
	return gpio_read(s->regs, 1 << s->offset);
}

static inline void pin_tgl_level(const struct gpio_pin_s* s) {
	gpio_toggle(s->regs, 1 << s->offset);
}

__END_DECLS

#endif