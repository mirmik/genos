#include "drivers/gpio.h"
#include "debug/dprint.h"
#include <util/bits.h>
#include <assert.h>

void gpio_set_level(struct gpio_regs* g, gpio_mask_t mask, unsigned char level) {
	if (level) g->ODR |= mask;
	else g->ODR &= ~mask;	
}

gpio_mask_t gpio_get_level(struct gpio_regs* g, gpio_mask_t mask, unsigned char level) {
	return g->ODR & mask;
}

void gpio_tgl_level(struct gpio_regs* g, gpio_mask_t mask) {
//	g->pin = mask;
}

//int gpio_settings(struct gpio_regs* g, gpio_mask_t mask, int32_t mode) {
//	if ((mode & GPIO_MODE_OUT_SECTION) &&
//		(mode & GPIO_MODE_IN_SECTION)) { /* mode is incorrect */
//		return -1;
//	}
	
//	uint8_t bit;
//	uint32_t nmoder = 0;
//	while(mask) {
//		bit = __builtin_ctz(mask);
//		nmoder |= (1 << (bit*2));
//		mask &= ~(1 << bit);
//	}

//	if (mode & GPIO_MODE_INPUT) {
//		g->MODER &= ~nmoder; 
//		return 0;
//	};

//	if (mode & GPIO_MODE_OUTPUT) {
//		g->MODER |= nmoder;		
//		return 0;
//	};

//	return -1;
//};

int gpio_settings(struct gpio_regs * gpio, gpio_mask_t mask, int mode) {
	int mode_val = 0;
	assert(gpio);

	if ((mode & GPIO_MODE_OUT_SECTION) &&
		(mode & GPIO_MODE_IN_SECTION)) { /* mode is incorrect */
		return -1;
	}

	if (mode & GPIO_MODE_INPUT) {

		mode_val = 0x00;
		/* mask inputs flag only without GPIO_MODE_INPUT */
		//if ((mode & GPIO_MODE_IN_SECTION) & ~GPIO_MODE_INPUT) {
		//	mode_val = 8;

		//	if (mode & GPIO_MODE_IN_PULL_UP) {
				//REG_STORE(&(gpio->bsrr), mask);
		//	}

		//	if (mode & GPIO_MODE_IN_PULL_DOWN) {
				//REG_STORE(&(gpio->brr), mask);
		//	}
		//}
	} else if (mode & GPIO_MODE_OUTPUT) {

		mode_val = 0b01;

		//if (mode & GPIO_MODE_OUT_OPEN_DRAIN) {
		//	mode_val |= 4;
		//}
	
	} else if (mode & GPIO_MODE_ALTERNATE) {
	
		mode_val = 0b10;
	}


	gpio->MODER = bits_multimap_uint32(mask, mode_val, 2);

	return 0;
}

int gpio_settings_alternate(struct gpio_regs *g, gpio_mask_t mask, int32_t alternate) {
	uint16_t lmask = (mask & 0x0000FFFF);
	uint16_t hmask = (mask & 0xFFFF0000) >> 16;
	alternate = alternate & 0xF;
	g->AFR[0] &= (~bits_multimap_uint32(lmask, 0xF, 4));
	g->AFR[1] &= (~bits_multimap_uint32(hmask, 0xF, 4));
	g->AFR[0] |= bits_multimap_uint32(lmask, alternate, 4);
	g->AFR[1] |= bits_multimap_uint32(hmask, alternate, 4);
}