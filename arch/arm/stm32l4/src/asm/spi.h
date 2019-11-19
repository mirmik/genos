#ifndef ARCH_STM32L4_SPI_H
#define ARCH_STM32L4_SPI_H

#include <stdint.h>
#include <stdbool.h>
#include <sys/cdefs.h>

#include <igris/util/bits.h>
#include <periph/map.h>

#include <stdint.h>

#include <asm/rcc.h>
#include <igris/util/bug.h>

#include <systime/systime.h>

__BEGIN_DECLS

static inline
void stm32l4_spi_enable(SPI_TypeDef * regs, bool en)
{
	BUG();
	rcc_enable_spi(regs);
	regs->CR1 |= SPI_CR1_SPE;
}

static inline
void stm32l4_spi_begin(SPI_TypeDef * regs)
{
	rcc_enable_spi(regs);
	//stm32l4_spi_enable(regs, en);
	delayMicroseconds(100000);

//	regs->CR2 = 0;
	//regs->CR2 = SPI_CR2_FRXTH;
//	regs->CR1 = SPI_CR1_SPE | SPI_CR1_MSTR | SPI_CR1_BR_2 | SPI_CR1_SSI | SPI_CR1_SSM;
	regs->CR1 = SPI_CR1_MSTR | SPI_CR1_BR_1
	            | SPI_CR1_BR_2 | SPI_CR1_BR_0 | SPI_CR1_SSM | SPI_CR1_SSI;
//regs->CR2 = SPI_CR2_SSOE;// |SPI_CR2_RXNEIE;
	regs->CR2 = SPI_CR2_FRXTH;
	regs->CR1 |= SPI_CR1_SPE;
	delayMicroseconds(100000);

	//delayMicroseconds(100000);

//	return 0;
}

static inline
void stm32l4_spi_set_divider_code(SPI_TypeDef * regs, uint8_t code)
{
	bits_assign_bias(regs->CR1, 0b111, code, 3);
}

static inline
int stm32l4_spi_set_divider(SPI_TypeDef * regs, int divider)
{
	int divcode;

	switch (divider)
	{
		case 2: divcode = 0b000;
		case 4: divcode = 0b001;
		case 8: divcode = 0b010;
		case 16: divcode = 0b011;
		case 32: divcode = 0b100;
		case 64: divcode = 0b101;
		case 128: divcode = 0b110;
		case 256: divcode = 0b111;
		default: return -1;
	}

	stm32l4_spi_set_divider_code(regs, divcode);

	return 0;
}

// if rxbuf == NULL, - send only
// rxbuf can be equal to txbuf
void stm32l4_spi_exchange(SPI_TypeDef * spi,
                          const uint8_t* sbuf, uint8_t *rbuf, uint32_t len, char dummy);


__END_DECLS

#endif