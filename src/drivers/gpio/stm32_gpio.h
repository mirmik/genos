#ifndef DRIVERS_STM32_GPIO_H
#define DRIVERS_STM32_GPIO_H

#include <stdint.h>
#include <asm/stm32_gpio.h>

#include <drivers/gpio/gpio_device.h>

class stm32_gpio_device : public gpio_device
{
public:	
	void set(unsigned int mask, uint8_t level) override;
	void toggle(unsigned int mask) override;
	unsigned int get(unsigned int mask) override;
	int mode(unsigned int mask, uint32_t mode, int arg) override;
};

#endif