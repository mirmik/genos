#include <drivers/gpio/avr_gpio.h>

int avr_gpio_device::setup(unsigned int mask, uint32_t mode) 
{
	BUG();
}

void avr_gpio_device::write(unsigned int mask, uint8_t level) 
{
	BUG();
}

void avr_gpio_device::toggle(unsigned int mask) 
{
	BUG();
}

unsigned int avr_gpio_device::read(unsigned int mask) 
{
	BUG();
}

void avr_gpio_device::pull_up(unsigned int mask, uint8_t level) 
{
	BUG();
}	
