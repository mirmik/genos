/**
	@file
	Контейнер для gpio процессора и маски.
*/

#ifndef ZILLOT_GPIO_PIN_H
#define ZILLOT_GPIO_PIN_H

#include <zillot/common/gpio.h>

namespace zillot 
{
	class pin
	{
		zillot::gpio * gpio;
		unsigned int mask;

	public:
		pin(zillot::gpio * gpio, unsigned int mask) : gpio(gpio), mask(mask) {}
		pin() : gpio(nullptr), mask(0) {}
	
		void set(int lvl) const { gpio->write(mask, lvl); }
		void pull_up(int lvl) const { gpio->pull_up(mask, lvl); }
		void toggle() const { gpio->toggle(mask); }
		uint8_t get() const { return !!gpio->read(mask); }
		int setup(uint32_t _mode) const { return gpio->setup(mask, _mode); }
	};
};

#endif