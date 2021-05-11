#ifndef ASM_STM32_GPIO_H
#define ASM_STM32_GPIO_H

#include <sys/cdefs.h>
#include <periph/map.h>

typedef GPIO_TypeDef gpio_regs_t;

#if defined CHIP_STM32F1XX || defined CHIP_STM32F2XX
enum stm32_gpio_maxspeed_e 
{
	STM32_GPIO_2MHZ = 0b10,
	STM32_GPIO_10MHZ = 0b01,
	STM32_GPIO_50MHZ = 0b11
};
#elif defined CHIP_STM32F3XX || defined CHIP_STM32F4XX \
	|| defined CHIP_STM32L4XX || defined STM32G4XX || defined STM32F7XX || defined STM32F4XX 
enum stm32_gpio_maxspeed_e 
{
	STM32_GPIO_SPEED_LEVEL_0 = 0b00,
	STM32_GPIO_SPEED_LEVEL_1 = 0b01,
	STM32_GPIO_SPEED_LEVEL_2 = 0b10,
	STM32_GPIO_SPEED_LEVEL_3 = 0b11,
};
#endif

__BEGIN_DECLS

void stm32_gpio_set_output(GPIO_TypeDef * gpio, unsigned int mask);
void stm32_gpio_set_level(GPIO_TypeDef * gpio, unsigned int mask, uint8_t lvl);

void stm32_gpio_write(GPIO_TypeDef * gpio, unsigned int mask, uint8_t lvl);
unsigned int stm32_gpio_read(GPIO_TypeDef * gpio, unsigned int mask);
void stm32_gpio_toggle(GPIO_TypeDef * gpio, unsigned int mask);

int stm32_gpio_set_alternate(GPIO_TypeDef* g, uint32_t mask, int32_t alternate);

void stm32_gpio_set_maxspeed(GPIO_TypeDef* g, uint16_t map,
                             enum stm32_gpio_maxspeed_e maxspeed);

void stm32_gpio_pull_up(GPIO_TypeDef * gpio, unsigned int mask, uint8_t lvl);

int stm32_gpio_mode(GPIO_TypeDef * gpio, unsigned int mask, uint32_t mode); 

__END_DECLS

#endif