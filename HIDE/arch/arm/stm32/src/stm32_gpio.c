#include <asm/stm32_gpio.h>
#include <defs/gpiodefs.h>

#include <igris/util/bits.h>

void stm32_gpio_set_output(GPIO_TypeDef * gpio, unsigned int mask)
{
	uint8_t mode_val = 0b01;

	bits_clr(gpio->OTYPER, mask);
	bits_masked_assign_multimap(gpio->OSPEEDR, mask, 0b11, 2);
	bits_masked_assign_multimap(gpio->MODER, mask, mode_val, 2);
}

void stm32_gpio_set_level(GPIO_TypeDef * gpio, unsigned int mask, uint8_t lvl)
{
	if (lvl) gpio->ODR |= mask;
	else gpio->ODR &= ~mask;
}

void stm32_gpio_write(GPIO_TypeDef * gpio, unsigned int mask, uint8_t lvl)
{
	if (lvl) gpio->ODR |= mask;
	else gpio->ODR &= ~mask;
}

unsigned int stm32_gpio_read(GPIO_TypeDef * gpio, unsigned int mask)
{
	return gpio->IDR & mask;
}

void stm32_gpio_toggle(GPIO_TypeDef* g, unsigned int mask)
{
	g->ODR ^= mask;
}

int stm32_gpio_mode(GPIO_TypeDef* gpio, unsigned int mask, uint32_t mode)
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

void stm32_gpio_pull_up(GPIO_TypeDef * gpio, unsigned int mask, uint8_t lvl)
{
	if (lvl)
		bits_masked_assign_multimap(gpio->PUPDR, mask, 0b01, 2);

	else
		bits_masked_assign_multimap(gpio->PUPDR, mask, 0b10, 2);
}


int stm32_gpio_set_alternate(GPIO_TypeDef* g, uint32_t mask, int32_t alternate)
{
	uint16_t lmask = (mask & 0x00FF);
	uint16_t hmask = (mask & 0xFF00) >> 8;
	alternate = alternate & 0xF;
	bits_masked_assign_multimap(g->AFR[0], lmask, alternate, 4);
	bits_masked_assign_multimap(g->AFR[1], hmask, alternate, 4);

	return 0;
}

void stm32_gpio_set_maxspeed(GPIO_TypeDef* regs, uint16_t map,
                             enum stm32_gpio_maxspeed_e maxspeed)
{
#if defined CHIP_STM32F1XX || defined CHIP_STM32F2XX 

	uint8_t hmap = (map & 0xFF00) >> 8;
	uint8_t lmap = map & 0x00FF;

	bits_masked_assign_multimap(regs->CRL, lmap, maxspeed, 4);
	bits_masked_assign_multimap(regs->CRH, hmap, maxspeed, 4);

#elif defined CHIP_STM32F3XX || \
	defined CHIP_STM32F4XX || \
	defined CHIP_STM32L4XX || \
	defined STM32F4XX || \
	defined STM32G4XX || \
	defined STM32F7XX
	bits_masked_assign_multimap(regs->OSPEEDR, map, maxspeed, 2);

#else
#	error "Wrong family"
#endif
}