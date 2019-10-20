#ifndef STM32L4_RCC_H
#define STM32L4_RCC_H

#include <sys/cdefs.h>
#include <periph/map.h>

__BEGIN_DECLS

void stm32l4_rcc_enable_gpio(GPIO_TypeDef * gpio);
void stm32l4_rcc_enable_usart(USART_TypeDef * usart);
void stm32l4_rcc_enable_spi(SPI_TypeDef * spi);

void stm32l4_rcc_reset();

static inline void rcc_enable_spi(SPI_TypeDef * spi) { stm32l4_rcc_enable_spi(spi); };
static inline void rcc_enable_gpio(GPIO_TypeDef * spi) { stm32l4_rcc_enable_gpio(spi); };

__END_DECLS

#endif