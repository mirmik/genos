#ifndef ARCH_STM32_SPI_H
#define ARCH_STM32_SPI_H

#include <stdint.h>
#include <stdbool.h>
#include <sys/cdefs.h>

#include <igris/dprint.h>
#include <igris/util/bits.h>
#include <periph/map.h>
#include <asm/stm32_rcc.h>

#if SPI_DEBUG >= 1
#define spidebug(...) dprln(__VA_ARGS__)
#else
#define spidebug(...)
#endif 

#if SPI_DEBUG >= 2
#define spidebug2(...) dprln(__VA_ARGS__)
#else
#define spidebug2(...)
#endif 

typedef SPI_TypeDef spi_regs_t;

__BEGIN_DECLS

static inline
void stm32_spi_rcc_enable(SPI_TypeDef * regs, bool en)
{
	stm32_rcc_enable_spi(regs);
}

static inline
void stm32_spi_set_master_mode(SPI_TypeDef * regs, bool en)
{
	if (en)
		regs->CR1 |= SPI_CR1_MSTR;
	else
		regs->CR1 &= ~SPI_CR1_MSTR;
}

static inline
void stm32_spi_enable(SPI_TypeDef * regs, bool en)
{
	regs->CR1 |= SPI_CR1_SPE;
}

static inline
void stm32_spi_set_divider_code(SPI_TypeDef * regs, uint8_t code)
{
	bits_assign_bias(regs->CR1, 0b111, code, 3);
}

static inline
int stm32_spi_set_divider(SPI_TypeDef * regs, int divider)
{
	int divcode;

	switch (divider)
	{
		case 2: divcode = 0b000; break;
		case 4: divcode = 0b001; break;
		case 8: divcode = 0b010; break;
		case 16: divcode = 0b011; break;
		case 32: divcode = 0b100; break;
		case 64: divcode = 0b101; break;
		case 128: divcode = 0b110; break;
		case 256: divcode = 0b111; break;
		default: return -1;
	}

	stm32_spi_set_divider_code(regs, divcode);

	return 0;
}

// if rxbuf == NULL, - send only
// rxbuf can be equal to txbuf
//void stm32_spi_exchange(SPI_TypeDef * spi,
//	const uint8_t* sbuf, uint8_t *rbuf, uint32_t len, char dummy);

void stm32_spi_send_byte(SPI_TypeDef * regs, uint8_t byte);
uint8_t stm32_spi_recv_byte(SPI_TypeDef * regs);

static inline
void stm32_spi_wait_for_tx_empty(SPI_TypeDef * regs)
{
	while (!(regs->SR & SPI_SR_TXE)) 
	{
		spidebug2("WAIT !TXE:", regs->SR);
	}
}

static inline
void stm32_spi_wait_for_rx_not_empty(SPI_TypeDef * regs)
{
	while (!(regs->SR & SPI_SR_RXNE)) 
	{
		spidebug2("WAIT !RXNE:", regs->SR);
	}
}

static inline 
void stm32_spi_wait_until_busy(SPI_TypeDef * regs)
{
	while (regs->SR & SPI_SR_BSY) 
	{
		spidebug2("WAIT BSY:", regs->SR);
	} 
}

static inline 
void stm32_spi_set_mode(SPI_TypeDef * regs, bool cpol, bool cpha) 
{
	uint8_t dat = (((uint8_t)cpol) << 1) | (((uint8_t)cpha) << 0);

	bits_assign_bias(regs->CR1, 0b11, dat, 0);
}

static inline 
void stm32_spi_debug_print(SPI_TypeDef * regs) 
{
	dpr("SPI SR:");
	dprhex((uint16_t)regs->SR); 
	dpr(" CR1:");
	dprhex((uint16_t)regs->CR1);
	dpr(" CR2:");
	dprhex((uint16_t)regs->CR2);
	dln();
}

static inline
void stm32_spi_disable_nss(SPI_TypeDef * regs, bool en) 
{
	regs->CR1 |= SPI_CR1_SSM;
	regs->CR1 |= SPI_CR1_SSI;
}

__END_DECLS

#endif