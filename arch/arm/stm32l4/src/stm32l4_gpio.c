#include <asm/gpio.h>
#include <drivers/gpio/gpio.h>
#include <igris/util/bits.h>

void stm32l4_gpio_write(GPIO_TypeDef* g, uint32_t mask,
                      unsigned char level)
{
	if (level) g->ODR |= mask;
	else g->ODR &= ~mask;
}

void gpio_write(GPIO_TypeDef* g, uint32_t mask, unsigned char level) 
{
	stm32l4_gpio_write(g, mask, level);
}

uint32_t stm32l4_gpio_read(GPIO_TypeDef* g, uint32_t mask)
{
	return g->IDR & mask;
}

uint32_t gpio_read(GPIO_TypeDef* g, uint32_t mask) 
{
	return  stm32l4_gpio_read(g, mask);
}

inline void stm32l4_gpio_toggle(GPIO_TypeDef* g, uint32_t mask)
{
	g->ODR ^= mask;
}

void gpio_toggle(GPIO_TypeDef* g, uint32_t mask) 
{
	stm32l4_gpio_toggle(g, mask);
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

int gpio_settings(GPIO_TypeDef* gpio, unsigned int mask, uint32_t mode) 
{
	stm32l4_gpio_mode(gpio, mask, mode);
}

int stm32l4_gpio_set_alternate(GPIO_TypeDef* g, uint32_t mask, int32_t alternate)
{
	uint16_t lmask = (mask & 0x00FF);
	uint16_t hmask = (mask & 0xFF00) >> 8;
	alternate = alternate & 0xF;
	bits_masked_assign_multimap(g->AFR[0], lmask, alternate, 4);
	bits_masked_assign_multimap(g->AFR[1], hmask, alternate, 4);

	return 0;
}
/*
void stm32_gpio_set_maxspeed(GPIO_TypeDef* regs, uint16_t map,
                             enum stm32_gpio_maxspeed_e maxspeed)
{
	bits_masked_assign_multimap(regs->OSPEEDR, map, maxspeed, 2);
}*/