#ifndef HAL_DRIVERS_GPIO_H
#define HAL_DRIVERS_GPIO_H

#include <periph/regs/gpio.h>
#include <periph/map.h>

#include <igris/util/bits.h>

#include <igris/dprint.h>

//#define GPIO_MODE_IN_PULL_UP             	((int32_t)2 << 0)
//#define GPIO_MODE_IN_PULL_DOWN             	((int32_t)4 << 0)
//#define GPIO_MODE_OUT_OPEN_DRAIN            ((int32_t)2 << 16)
//#define GPIO_MODE_ALTERNATE	            	((int32_t)1 << 24)

//int gpio_settings_alternate(struct gpio_regs *g, gpio_mask_t mask, int32_t alternate);

#if defined CHIP_STM32F1XX || defined CHIP_STM32F2XX
enum stm32_gpio_maxspeed_e 
{
	STM32_GPIO_2MHZ = 0b10,
	STM32_GPIO_10MHZ = 0b01,
	STM32_GPIO_50MHZ = 0b11
};
#elif defined CHIP_STM32F3XX || defined CHIP_STM32F4XX \
	|| defined CHIP_STM32L4XX
enum stm32_gpio_maxspeed_e 
{
	STM32_GPIO_SPEED_LEVEL_0 = 0b00,
	STM32_GPIO_SPEED_LEVEL_1 = 0b01,
	STM32_GPIO_SPEED_LEVEL_2 = 0b10,
	STM32_GPIO_SPEED_LEVEL_3 = 0b11,
};
#endif

__BEGIN_DECLS

void stm32_gpio_set_maxspeed(struct gpio_regs* regs, uint16_t map,
                             enum stm32_gpio_maxspeed_e maxspeed);

static inline
void stm32_gpio_write(struct gpio_regs* g, uint32_t mask,
                      unsigned char level)
{
	if (level) g->ODR |= mask;
	else g->ODR &= ~mask;
}

static inline
uint32_t stm32_gpio_read(struct gpio_regs* g, uint32_t mask)
{
	return g->IDR & mask;
}

static inline
void stm32_gpio_toggle(struct gpio_regs* g, uint32_t mask)
{
	g->ODR ^= mask;
}

static inline
int stm32_gpio_set_alternate(struct gpio_regs *g, uint32_t mask, int32_t alternate)
{
	uint16_t lmask = (mask & 0x00FF);
	uint16_t hmask = (mask & 0xFF00) >> 8;
	alternate = alternate & 0xF;
	bits_masked_assign_multimap(g->AFR[0], lmask, alternate, 4);
	bits_masked_assign_multimap(g->AFR[1], hmask, alternate, 4);

	return 0;
}

static inline void stm32_afr_debug_print(struct gpio_regs *g) 
{
	DPRINTHEX(g->AFR[0]);
	DPRINTHEX(g->AFR[1]);
}

__END_DECLS

#endif