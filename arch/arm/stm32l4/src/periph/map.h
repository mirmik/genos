#ifndef PERIPH_MAP_H
#define PERIPH_MAP_H

#if defined(CHIP_STM32L412XX)
#	include <cmsis/stm32l412xx.h>
#elif defined(CHIP_STM32L432XX)
#	include <cmsis/stm32l432xx.h>
#else
#	error "unregistred CHIP_STM32L4 chip" 
#endif

typedef GPIO_TypeDef gpio_regs_t;
typedef USART_TypeDef usart_regs_t;
typedef SPI_TypeDef spi_regs_t;

#endif