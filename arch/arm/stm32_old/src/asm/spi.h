#ifndef ARCH_STM32_SPI_H
#define ARCH_STM32_SPI_H

#include <stdint.h>
#include <stdbool.h>
#include <sys/cdefs.h>

#include <igris/util/bits.h>
#include <periph/regs/spi.h>

typedef struct spi_regs spi_regs_t;

__BEGIN_DECLS

static inline
void stm32_spi_enable(struct spi_regs * regs, bool en) 
{
	regs->CR1 |= SPI_CR1_SPE;
}

static inline 
void stm32_spi_set_divider_code(struct spi_regs * regs, uint8_t code) 
{
	bits_assign_bias(regs->CR1, 0b111, code, 3);
}

static inline 
int stm32_spi_set_divider(struct spi_regs * regs, int divider) 
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

	stm32_spi_set_divider_code(regs, divcode);

	return 0;
}

// if rxbuf == NULL, - send only
// rxbuf can be equal to txbuf
void stm32_spi_exchange(struct spi_regs * spi, 
	const uint8_t* sbuf, uint8_t *rbuf, uint32_t len, char dummy);


__END_DECLS

#endif