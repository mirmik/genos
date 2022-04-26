#ifndef GENOS_DRIVERS_AVR_GPIO_H
#define GENOS_DRIVERS_AVR_GPIO_H

#include <drivers/gpio/gpio_device.h>
#include <asm/avr_gpio.h>

namespace genos
{
	class avr_gpio : public gpio_device
	{
	public:
		gpio_regs * regs;

	public:
		avr_gpio(gpio_regs * regs);

		int set(uint32_t mask, uint32_t level) override;
		int toggle(uint32_t mask) override;
		int get(uint32_t mask, uint32_t* result) override;

		int mode(uint32_t mask, uint32_t mode, int arg) override;
	};
}

#endif