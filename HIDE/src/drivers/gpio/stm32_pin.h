#ifndef GENOS_STM32_PIN_H
#define GENOS_STM32_PIN_H

#include <stdint.h>
#include <drivers/gpio/abstract_gpio_pin.h>

class GPIO_TypeDef;

namespace genos 
{
	class stm32_pin : public abstract_gpio_pin
	{
		GPIO_TypeDef * _dev;
		uint32_t      _mask;

	public:
		stm32_pin(GPIO_TypeDef * dev, uint32_t mask) :
			_dev(dev), _mask(mask)
		{}

		int set(unsigned char level) override;;
		int get() override;;
		int toggle() override;;
		int mode(uint32_t mode, int arg) override;
	};
}

#endif