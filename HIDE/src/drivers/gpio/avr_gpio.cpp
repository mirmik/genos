#include <drivers/gpio/avr_gpio.h>
#include <asm/avr_gpio.h>

genos::avr_gpio::avr_gpio(struct gpio_regs * regs)
	: regs(regs)
{}

int genos::avr_gpio::mode(uint32_t mask, uint32_t mode, int arg)
{
	return avr_gpio_mode(regs, mask, mode);
}

int genos::avr_gpio::set(uint32_t mask, uint32_t level)
{
	return avr_gpio_set(regs, mask, level);
}

int genos::avr_gpio::get(uint32_t mask, uint32_t * result)
{
	return avr_gpio_get(regs, mask);
}

int genos::avr_gpio::toggle(uint32_t mask)
{
	return avr_gpio_toggle(regs, mask);
}

