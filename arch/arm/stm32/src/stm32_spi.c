#include <asm/stm32_spi.h>
#include <igris/dprint.h>

static inline void stm32_spi_rd8(SPI_TypeDef *SPI, void *rx_data)
{
    *((uint8_t*)rx_data) = *((volatile uint8_t*)&SPI->DR);
}

static inline void stm32_spi_wr8(SPI_TypeDef *SPI, const void *tx_data)
{
    *((volatile uint8_t*)&SPI->DR) = *((const uint8_t*)tx_data);
}
/*
void stm32_spi_exchange(SPI_TypeDef * spi, 
	const uint8_t* sbuf, uint8_t *rbuf, uint32_t len)
{
	char c;

	while (len--)
	{
		while ((spi->SR & SPI_SR_RXNE)) {};
		while (!(spi->SR & SPI_SR_TXE)) {};

		stm32l4_spi_wr8(spi, sbuf++);

		while (!(spi->SR & SPI_SR_RXNE)) {};

		stm32l4_spi_rd8(spi, &c);
		if (rbuf) {
			*rbuf++ = c;
		}
	}

	while (spi->SR & SPI_SR_BSY); // Wait until the transmission of the last byte is complete
}*/

void stm32_spi_send_byte(SPI_TypeDef * regs, uint8_t byte) 
{
    regs->DR = byte;
}

uint8_t stm32_spi_recv_byte(SPI_TypeDef * regs) 
{
    return regs->DR;
}