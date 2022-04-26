#ifndef GENOS_AVR_PIN_H
#define GENOS_AVR_PIN_H

#include <drivers/gpio/abstract_gpio_pin.h>

class gpio_regs;

namespace genos 
{
	class avr_pin : public abstract_gpio_pin
	{
		gpio_regs * _dev;
		uint32_t      _mask;

	public:
		avr_pin(gpio_regs * dev, uint32_t mask) :
			_dev(dev), _mask(mask)
		{}

		int set(unsigned char level) override;;
		int get() override;;
		int toggle() override;;
		int mode(uint32_t mode, int arg) override;
	};
}

#endif