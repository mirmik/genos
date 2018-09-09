#include <drivers/gpio/gpio.h>

inline void gpio_set_level(struct gpio* g, gpio_mask_t mask, uint8_t level) {
	if (level) g->port |= mask;
	else g->port &= ~mask;	
}

inline gpio_mask_t gpio_get_level(struct gpio* g, gpio_mask_t mask) {
	return g->pin;
}

inline void gpio_tgl_level(struct gpio* g, gpio_mask_t mask) {
	g->pin = mask;
}

int gpio_settings(struct gpio* g, gpio_mask_t mask, uint32_t mode) {
	if ((mode & GPIO_MODE_OUT_SECTION) &&
		(mode & GPIO_MODE_IN_SECTION)) { /* mode is incorrect */
		return -1;
	}
	
	if (mode & GPIO_MODE_INPUT) {
		g->ddr &= ~mask; 
		return 0;
	};

	if (mode & GPIO_MODE_OUTPUT) {
		g->ddr |= mask;		
		return 0;
	};
}