#include <drivers/gpio/gpio.h>
#include <periph/map.h>

inline void gpio_write(gpio_regs_t* g, unsigned int mask, uint8_t level)
{
	if (level) ((struct gpio_regs *)(g))->port |= mask;
	else ((struct gpio_regs *)(g))->port &= ~mask;
}

inline unsigned int gpio_read(gpio_regs_t* g, unsigned int mask)
{
	return ((struct gpio_regs *)(g))->pin;
}

inline void gpio_toggle(gpio_regs_t* g, unsigned int mask)
{
	((struct gpio_regs *)(g))->pin = mask;
}

int gpio_settings(gpio_regs_t* g, unsigned int mask, uint32_t mode)
{
	if ((mode & GPIO_MODE_OUT_SECTION) &&
	        (mode & GPIO_MODE_IN_SECTION))   /* mode is incorrect */
	{
		return -1;
	}

	if (mode & GPIO_MODE_INPUT)
	{
		((struct gpio_regs *)(g))->ddr &= ~mask;
		return 0;
	};

	if (mode & GPIO_MODE_OUTPUT)
	{
		((struct gpio_regs *)(g))->ddr |= mask;
		return 0;
	};

	return -1;
}