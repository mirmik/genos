#ifndef GENOS_GPIO_TABLE_H
#define GENOS_GPIO_TABLE_H

#include "inttypes.h"
#include <sys/cdefs.h>
#include "drivers/gpio.h"

__BEGIN_DECLS

static inline void pinnum_settings(uint8_t num, int32_t mode) {
	pin_settings(&gpio_table[num], mode);
}

static inline void pinnum_set_level(uint8_t num, uint8_t lvl) {
	pin_set_level(&gpio_table[num], lvl);
}

static inline gpio_mask_t pinnum_get_level(uint8_t num) {
	return pin_get_level(&gpio_table[num]);
}

static inline void pinnum_tgl_level(uint8_t num) {
	pin_tgl_level(&gpio_table[num]);
}

__END_DECLS

#endif