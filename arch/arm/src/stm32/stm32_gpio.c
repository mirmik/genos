#include <stm32/gpio.h>
#include <igris/util/bits.h>

#include <asm/chip.h>

void stm32_gpio_set_maxspeed(struct gpio_regs* regs, uint16_t map,
                             enum stm32_gpio_maxspeed_e maxspeed)
{
#if defined CHIP_STM32F1XX || defined CHIP_STM32F2XX

	uint8_t hmap = (map & 0xFF00) >> 8;
	uint8_t lmap = map & 0x00FF;

	bits_masked_assign_multimap(regs->CRL, lmap, maxspeed, 4);
	bits_masked_assign_multimap(regs->CRH, hmap, maxspeed, 4);

#elif defined CHIP_STM32F3XX || defined CHIP_STM32F4XX

	bits_masked_assign_multimap(regs->OSPEEDR, map, maxspeed, 2);

#else
#	error "Wrong family"
#endif
}