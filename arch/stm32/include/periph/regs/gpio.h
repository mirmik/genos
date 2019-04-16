#ifndef STM32_GPIO_REGS_H
#define STM32_GPIO_REGS_H

#include <inttypes.h>

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

#endif