#include <asm/spi.h>
#include <igris/dprint.h>

void stm32l4_spi_exchange(SPI_TypeDef * spi, 
	const uint8_t* sbuf, uint8_t *rbuf, uint32_t len, char dummy)
{
	char c;

	DPRINT(len);
	while (len--)
	{
		//dprln("here");
		DPRINTHEX(*sbuf);

		//dprln("len--");
		while (!(spi->SR & SPI_SR_TXE)) 
		{
		}; // Wait until TX buffer is empty
		//dprln("here");
		spi->DR = *sbuf++;// : dummy; // Send byte (TXE cleared)

		while (!(spi->SR & SPI_SR_RXNE)) 
		{}; // Wait while RX buffer is empty
		//if (rbuf) {
		while ((spi->SR & SPI_SR_RXNE)) {
			c = spi->DR; // Read received byte
			DPRINTHEX(c);
			*rbuf++ = c;
		}
		//}
	}

	//DPRINTHEX(c);

	while (spi->SR & SPI_SR_BSY); // Wait until the transmission of the last byte is complete
}