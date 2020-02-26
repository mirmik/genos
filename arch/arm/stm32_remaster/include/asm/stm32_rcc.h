#ifndef ASM_STM32_RCC_H
#define ASM_STM32_RCC_H

#include <sys/cdefs.h>
#include <periph/map.h>

__BEGIN_DECLS

void stm32_rcc_enable_gpio(GPIO_TypeDef * gpio);
void stm32_rcc_enable_usart(USART_TypeDef * usart);
void stm32_rcc_enable_spi(SPI_TypeDef * spi);

void stm32_rcc_reset();

__END_DECLS

#endif