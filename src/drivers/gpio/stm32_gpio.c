#include <drivers/gpio/gpio.h>
#include <defs/gpiodefs.h>
#include <igris/util/bits.h>
#include <assert.h>

#include <asm/gpio.h>
#include <asm/stm32_gpio.h>
#include <asm/stm32_rcc.h>

void gpio_write(gpio_regs_t* g, unsigned int mask, unsigned char level)
{
	stm32_gpio_write((struct gpio_regs *)(g), mask, level);
}

unsigned int gpio_read(gpio_regs_t* g, unsigned int mask)
{
	return stm32_gpio_read((struct gpio_regs *)(g), mask);
}

void gpio_toggle(gpio_regs_t* g, unsigned int mask)
{
	stm32_gpio_toggle((struct gpio_regs *)(g), mask);
}

int gpio_settings(gpio_regs_t* g, unsigned int mask, uint32_t mode)
{
	gpio_regs_t * gpio = (gpio_regs_t *)(g);
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

//#ifdef CHIP_STM32
void gpio_alternate(GPIO_TypeDef * gpio, unsigned int mask, uint8_t code) 
{
	//struct gpio_regs * gpio = (struct gpio_regs *)(g);
	stm32_rcc_enable_gpio(gpio);
	bits_masked_assign_multimap(gpio->MODER, mask, 0b10, 2);
	stm32_gpio_set_alternate(gpio, mask, code);	
} 
//#endif