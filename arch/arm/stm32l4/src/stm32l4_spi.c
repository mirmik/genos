#include <asm/spi.h>
#include <igris/dprint.h>

static inline void stm32l4_spi_rd8(SPI_TypeDef *SPI, void *rx_data)
{
    *((uint8_t*)rx_data) = *((volatile uint8_t*)&SPI->DR);
}

static inline void stm32l4_spi_wr8(SPI_TypeDef *SPI, const void *tx_data)
{
    *((volatile uint8_t*)&SPI->DR) = *((const uint8_t*)tx_data);
}

void stm32l4_spi_exchange(SPI_TypeDef * spi, 
	const uint8_t* sbuf, uint8_t *rbuf, uint32_t len, char dummy)
{
	char c;

	DPRINT(len);
	while (len--)
	{
		//dprln("here");
		DPRINTHEX(*sbuf);
		while ((spi->SR & SPI_SR_RXNE)) ;

		//dprln("len--");
		while (!(spi->SR & SPI_SR_TXE)) 
		{
		}; // Wait until TX buffer is empty
		stm32l4_spi_wr8(spi, sbuf++);// : dummy; // Send byte (TXE cleared)

		while (!(spi->SR & SPI_SR_RXNE)) 
		{}; // Wait while RX buffer is empty

		stm32l4_spi_rd8(spi, &c); // Read received byte
		DPRINTHEX(c);
		if (rbuf) {
			*rbuf++ = c;
		}
	}

	//DPRINTHEX(c);

	while (spi->SR & SPI_SR_BSY); // Wait until the transmission of the last byte is complete
}