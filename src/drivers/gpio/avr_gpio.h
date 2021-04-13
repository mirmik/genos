#ifndef GENOS_DRIVERS_AVR_GPIO_H
#define GENOS_DRIVERS_AVR_GPIO_H

#include <drivers/gpio/gpio_device.h>
#include <drivers/gpio/pin2.h>

#include <asm/avr_gpio.h> 

class avr_gpio_pin : public gpio_pin
{
private:
	gpio_regs * regs;
	uint16_t mask;

public:
	avr_gpio_pin(struct gpio_regs * regs, uint16_t mask) 
		: regs(regs), mask(mask)
	{}

	int mode(uint32_t mode) override
	{
		return avr_gpio_mode(regs, mask, mode);
	}
	
	int set(uint8_t level = 1) override
	{
		return avr_gpio_set(regs, mask, level);
	}
	
	int get() override
	{
		return !!avr_gpio_get(regs, mask);
	}

	int toggle() override
	{
		return avr_gpio_toggle(regs, mask);
	}
	
};

#endif