#include <asm/stm32_spi.h>
#include <igris/dprint.h>

/*static inline void stm32_spi_rd8(SPI_TypeDef *SPI, void *rx_data)
{
    *((uint8_t*)rx_data) = *((volatile uint8_t*)&SPI->DR);
}

static inline void stm32_spi_wr8(SPI_TypeDef *SPI, const void *tx_data)
{
    *((volatile uint8_t*)&SPI->DR) = *((const uint8_t*)tx_data);
}*/

void stm32_spi_send_byte(SPI_TypeDef * regs, uint8_t byte) 
{
    //spidebug("SPI:SEND(PRE) BYTE:", byte, "SR:", (uint8_t)regs->SR);
    (*(volatile uint8_t *) &regs->DR) = byte;
    
    //spidebug("SPI:SEND BYTE:", byte, "SR:", (uint8_t)regs->SR);
}

uint8_t stm32_spi_recv_byte(SPI_TypeDef * regs) 
{
    //spidebug("SPI:RECV(PRE) SR:", (uint8_t)regs->SR);
    uint8_t ret = *(volatile uint8_t*)&regs->DR;

    //spidebug("SPI:RECV BYTE:", ret, "SR:", (uint8_t)regs->SR);
    
    return ret;
}