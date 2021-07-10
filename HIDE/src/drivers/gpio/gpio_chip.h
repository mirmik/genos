/*#ifndef GENOS_DRIVERS_GPIO_CHIP_H
#define GENOS_DRIVERS_GPIO_CHIP_H

namespace drivers 
{
	class gpio_chip 
	{
	public:
		virtual void set_value(int offset, int lvl) = 0; 
		virtual void set_direction(int offset, int dir) = 0;
		
		virtual int value(int offset) = 0; 
		virtual int direction(int offset) = 0;
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

		int value() { return chip->value(offset); }
	};
}

#endif*/