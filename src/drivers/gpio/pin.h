/**
	@file
	Контейнер для gpio процессора и маски.

	@details
	Технически, хорошо бы иметь более абстрактную реализацию интерфейса аппаратных уровней. 
*/

#ifndef DRIVERS_GPIO_PIN_H
#define DRIVERS_GPIO_PIN_H

#include <drivers/gpio/gpio.h>

struct gpio_pin {
	struct gpio * gpio;
	gpio_mask_t mask;
};

#define GPIO_PIN(name, io, num) struct gpio_pin name = { io, 1<<(num) };

__BEGIN_DECLS

static inline int gpio_pin_settings(struct gpio_pin * pin, uint32_t mode) {
	return gpio_settings(pin->gpio, pin->mask, mode);
}

static inline void gpio_pin_init(struct gpio_pin * pin, struct gpio * io, uint8_t num) {
	pin->gpio = io;
	pin->mask = 1 << num;
}

static inline void gpio_pin_set_level(struct gpio_pin * ptr, uint8_t lvl) {
	gpio_set_level(ptr->gpio, ptr->mask, lvl);
}

static inline void gpio_pin_toggle(struct gpio_pin * ptr) {
	gpio_toggle(ptr->gpio, ptr->mask);
}

static inline uint8_t gpio_pin_get(struct gpio_pin * ptr) {
	return !!gpio_get_level(ptr->gpio, ptr->mask);
}

__END_DECLS

#endif