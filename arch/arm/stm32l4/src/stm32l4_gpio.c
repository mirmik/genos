#include <asm/gpio.h>
#include <drivers/gpio/gpio.h>
#include <igris/util/bits.h>

void stm32l4_gpio_write(GPIO_TypeDef* g, uint32_t mask,
                      unsigned char level)
{
	if (level) g->ODR |= mask;
	else g->ODR &= ~mask;
}

uint32_t stm32l4_gpio_read(GPIO_TypeDef* g, uint32_t mask)
{
	return g->IDR & mask;
}

void stm32l4_gpio_toggle(GPIO_TypeDef* g, uint32_t mask)
{
	g->ODR ^= mask;
}

int stm32l4_gpio_mode(GPIO_TypeDef* gpio, unsigned int mask, uint32_t mode)
{
	int mode_val = 0;

	if ((mode & GPIO_MODE_OUT_SECTION) &&
	        (mode & GPIO_MODE_IN_SECTION))   /* mode is incorrect */
	{
		return -1;
	}

	if (mode & GPIO_MODE_INPUT)
	{
		mode_val = 0b00;

		if (mode & GPIO_MODE_IN_NOPULL)
		{
			bits_masked_assign_multimap(gpio->PUPDR, mask, 0b00, 2);
		}

		else if (mode & GPIO_MODE_IN_PULL_UP)
		{
			bits_masked_assign_multimap(gpio->PUPDR, mask, 0b01, 2);
		}

		else if (mode & GPIO_MODE_IN_PULL_DOWN)
		{
			bits_masked_assign_multimap(gpio->PUPDR, mask, 0b10, 2);
		}
	}
	
	else if (mode & GPIO_MODE_OUTPUT)
	{
		mode_val = 0b01;

		if (mode & GPIO_MODE_OUT_PUSH_PULL)
		{
			bits_clr(gpio->OTYPER, mask);
		}

		else if (mode & GPIO_MODE_OUT_OPEN_DRAIN)
		{
			bits_set(gpio->OTYPER, mask);
		}
	}
	
	else if (mode & GPIO_MODE_ALTERNATE)
	{
		mode_val = 0b10;
	}

	else if (mode & GPIO_MODE_ANALOG_INPUT)
	{
		mode_val = 0b11;
	}

	bits_masked_assign_multimap(gpio->MODER, mask, mode_val, 2);

	return 0;
}