#ifndef GENOS_DRIVERS_GPIO_CHIP_H
#define GENOS_DRIVERS_GPIO_CHIP_H

#include <drivers/gpio/abstract.h>

namespace drivers 
{
	class gpio_chip //: public gpio_pin_abstract
	{
	public:
		virtual void set_value(int offset, int lvl) = 0; 
		virtual int get_value(int offset) = 0; 
		virtual void set_direction(int offset, int dir) = 0;
		virtual int get_direction(int offset) = 0;
	};

	class gpio_chip_pin
	{
		gpio_chip * chip;
		int offset;

	public:
		gpio_chip_pin(gpio_chip * chip, int offset)
			: chip(chip), offset(offset)
		{}

		gpio_chip_pin(const gpio_chip_pin& other)
			: chip(other.chip), offset(other.offset)
		{}

		void set_value(int lvl) { chip->set_value(offset, lvl); }
		void set_direction(int lvl) { chip->set_direction(offset, lvl); }
	};
}

#endif