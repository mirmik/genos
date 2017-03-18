#include "drivers/gpio.h"
#include "debug/dprint.h"
#include <util/bits.h>
#include <assert.h>

void gpio_set_level(struct gpio_regs* g, gpio_mask_t mask, unsigned char level) {
	if (level) g->ODR |= mask;
	else g->ODR &= ~mask;	
}

gpio_mask_t gpio_get_level(struct gpio_regs* g, gpio_mask_t mask) {
	return g->ODR & mask;
}

void gpio_tgl_level(struct gpio_regs* g, gpio_mask_t mask) {
	g->ODR ^= mask;	
}

int gpio_settings(struct gpio_regs * gpio, gpio_mask_t mask, int mode) {
	int mode_val = 0;
	assert(gpio);

	if ((mode & GPIO_MODE_OUT_SECTION) &&
		(mode & GPIO_MODE_IN_SECTION)) { /* mode is incorrect */
		return -1;
	}

	if (mode & GPIO_MODE_INPUT) {
		mode_val = 0x00;

	} else if (mode & GPIO_MODE_OUTPUT) {
		mode_val = 0b01;
	
	} else if (mode & GPIO_MODE_ALTERNATE) {
		mode_val = 0b10;
	}

	bits_mask_assign_multimap_uint32(gpio->MODER, mask, mode_val, 2);

	return 0;
}

int gpio_settings_alternate(struct gpio_regs *g, gpio_mask_t mask, int32_t alternate) {
	uint16_t lmask = (mask & 0x0000FFFF);
	uint16_t hmask = (mask & 0xFFFF0000) >> 16;
	alternate = alternate & 0xF;
	g->AFR[1] |= bits_multimap_uint32(hmask, alternate, 4);
	bits_mask_assign_multimap_uint32(g->AFR[0], lmask, 0x7, 4);
	bits_mask_assign_multimap_uint32(g->AFR[1], hmask, 0x7, 4);
}