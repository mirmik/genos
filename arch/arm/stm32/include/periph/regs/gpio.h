#ifndef STM32_GPIO_REGS_H
#define STM32_GPIO_REGS_H

#include <inttypes.h>
#include <periph/map.h>

typedef uint16_t __gpio_mask_t;

struct gpio_regs {
	volatile uint32_t MODER;    /*!< GPIO port mode register,               Address offset: 0x00      */
	volatile uint32_t OTYPER;   /*!< GPIO port output type register,        Address offset: 0x04      */
	volatile uint32_t OSPEEDR;  /*!< GPIO port output speed register,       Address offset: 0x08      */
	volatile uint32_t PUPDR;    /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C      */
	volatile uint32_t IDR;      /*!< GPIO port input data register,         Address offset: 0x10      */
	volatile uint32_t ODR;      /*!< GPIO port output data register,        Address offset: 0x14      */
	volatile uint16_t BSRRL;    /*!< GPIO port bit set/reset low register,  Address offset: 0x18      */
	volatile uint16_t BSRRH;    /*!< GPIO port bit set/reset high register, Address offset: 0x1A      */
	volatile uint32_t LCKR;     /*!< GPIO port configuration lock register, Address offset: 0x1C      */
	volatile uint32_t AFR[2];   /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */
};

#define GPIOA ((struct gpio_regs *) GPIOA_BASE)
#define GPIOB ((struct gpio_regs *) GPIOB_BASE)
#define GPIOC ((struct gpio_regs *) GPIOC_BASE)
#define GPIOD ((struct gpio_regs *) GPIOD_BASE)
#define GPIOE ((struct gpio_regs *) GPIOE_BASE)
#define GPIOF ((struct gpio_regs *) GPIOF_BASE)
#define GPIOG ((struct gpio_regs *) GPIOG_BASE)
#define GPIOH ((struct gpio_regs *) GPIOH_BASE)
#define GPIOI ((struct gpio_regs *) GPIOI_BASE)
#define GPIOJ ((struct gpio_regs *) GPIOJ_BASE)
#define GPIOK ((struct gpio_regs *) GPIOK_BASE)

#endif