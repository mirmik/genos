#ifndef DRIVERS_STM32_GPIO_H
#define DRIVERS_STM32_GPIO_H

#include <drivers/gpio/gpio_device.h>

struct GPIO_TypeDef;

namespace genos
{
	class stm32_gpio : public gpio_device
	{
	public:
		GPIO_TypeDef * gpio;

	public:
		stm32_gpio(GPIO_TypeDef * gpio);

		int set(uint32_t mask, uint32_t level) override;
		int toggle(uint32_t mask) override;
		int get(uint32_t mask, uint32_t * result) override;

		int mode(uint32_t mask, uint32_t mode, int arg) override;
	};
}

#endif