#include <drivers/gpio/avr_gpio_device.h>
#include <asm/avr_gpio.h>

avr_gpio_device::avr_gpio_device(struct gpio_regs * regs)
	: regs(regs)
{}

int avr_gpio_device::mode(uint32_t mask, uint32_t mode, int arg)
{
	return avr_gpio_mode(regs, mask, mode);
}

int avr_gpio_device::set(uint32_t mask, uint32_t level)
{
	return avr_gpio_set(regs, mask, level);
}

int avr_gpio_device::get(uint32_t mask, uint32_t * result)
{
	return avr_gpio_get(regs, mask);
}

int avr_gpio_device::toggle(uint32_t mask)
{
	return avr_gpio_toggle(regs, mask);
}

