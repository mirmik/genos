/**
	@file
	Контейнер для gpio процессора и маски.

	@details
	Технически, хорошо бы иметь более абстрактную реализацию интерфейса аппаратных уровней.
*/

#ifndef DRIVERS_GPIO_PIN_H
#define DRIVERS_GPIO_PIN_H

#include <drivers/gpio/gpio.h>
#include <asm/chip.h>

struct gpio_pin
{
	gpio_t * gpio;
	gpio_mask_t mask;

#ifdef __cplusplus
	gpio_pin(gpio_t * gpio, gpio_mask_t mask) : gpio(gpio), mask(mask) {}
	
	void set(int lvl) { gpio_write(gpio, mask, lvl); }
	void toggle() { gpio_toggle(gpio, mask); }
	void tgl() { gpio_toggle(gpio, mask); }
	uint8_t get() const { return !!gpio_read(gpio, mask); }
	int mode(uint32_t _mode) const { return gpio_settings(gpio, mask, _mode); }

	#ifdef CHIP_STM32
	void alternate(uint8_t fn) 
	{ 
		gpio_alternate(gpio, mask, fn);
	};
	#endif
#endif
};

#define GPIO_PIN(name, io, num) struct gpio_pin name = { io, 1<<(num) };

__BEGIN_DECLS

static inline int gpio_pin_settings(struct gpio_pin * pin, uint32_t mode)
{
	return gpio_settings(pin->gpio, pin->mask, mode);
}

static inline void gpio_pin_init(struct gpio_pin * pin, gpio_t * io, uint8_t num)
{
	pin->gpio = io;
	pin->mask = 1 << num;
}

static inline void gpio_pin_write(struct gpio_pin * ptr, uint8_t lvl)
{
	gpio_write(ptr->gpio, ptr->mask, lvl);
}

static inline void gpio_pin_toggle(struct gpio_pin * ptr)
{
	gpio_toggle(ptr->gpio, ptr->mask);
}

static inline uint8_t gpio_pin_read(struct gpio_pin * ptr)
{
	return !!gpio_read(ptr->gpio, ptr->mask);
}

__END_DECLS

#endif