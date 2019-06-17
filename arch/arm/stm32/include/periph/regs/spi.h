#ifndef STM32_SPI_REGS_H
#define STM32_SPI_REGS_H

#include <inttypes.h>

struct spi_regs
{
	volatile uint16_t CR1;        /*!< SPI control register 1 (not used in I2S mode),      Address offset: 0x00 */
	uint16_t RESERVED0;           /*!< Reserved, 0x02                                                           */
	volatile uint16_t CR2;        /*!< SPI control register 2,                             Address offset: 0x04 */
	uint16_t RESERVED1;           /*!< Reserved, 0x06                                                           */
	volatile uint16_t SR;         /*!< SPI status register,                                Address offset: 0x08 */
	uint16_t RESERVED2;           /*!< Reserved, 0x0A                                                           */
	volatile uint16_t DR;         /*!< SPI data register,                                  Address offset: 0x0C */
	uint16_t RESERVED3;           /*!< Reserved, 0x0E                                                           */
	volatile uint16_t CRCPR;      /*!< SPI CRC polynomial register (not used in I2S mode), Address offset: 0x10 */
	uint16_t RESERVED4;           /*!< Reserved, 0x12                                                           */
	volatile uint16_t RXCRCR;     /*!< SPI RX CRC register (not used in I2S mode),         Address offset: 0x14 */
	uint16_t RESERVED5;           /*!< Reserved, 0x16                                                           */
	volatile uint16_t TXCRCR;     /*!< SPI TX CRC register (not used in I2S mode),         Address offset: 0x18 */
	uint16_t RESERVED6;           /*!< Reserved, 0x1A                                                           */
	volatile uint16_t I2SCFGR;    /*!< SPI_I2S configuration register,                     Address offset: 0x1C */
	uint16_t RESERVED7;           /*!< Reserved, 0x1E                                                           */
	volatile uint16_t I2SPR;      /*!< SPI_I2S prescaler register,                         Address offset: 0x20 */
	uint16_t RESERVED8;           /*!< Reserved, 0x22                                                           */
};

/******************************************************************************/
/*                                                                            */
/*                        Serial Peripheral Interface                         */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for SPI_CR1 register  ********************/
#define  SPI_CR1_CPHA                        ((uint16_t)0x0001)            /*!<Clock Phase      */
#define  SPI_CR1_CPOL                        ((uint16_t)0x0002)            /*!<Clock Polarity   */
#define  SPI_CR1_MSTR                        ((uint16_t)0x0004)            /*!<Master Selection */

#define  SPI_CR1_BR                          ((uint16_t)0x0038)            /*!<BR[2:0] bits (Baud Rate Control) */
#define  SPI_CR1_BR_0                        ((uint16_t)0x0008)            /*!<Bit 0 */
#define  SPI_CR1_BR_1                        ((uint16_t)0x0010)            /*!<Bit 1 */
#define  SPI_CR1_BR_2                        ((uint16_t)0x0020)            /*!<Bit 2 */

#define  SPI_CR1_SPE                         ((uint16_t)0x0040)            /*!<SPI Enable                          */
#define  SPI_CR1_LSBFIRST                    ((uint16_t)0x0080)            /*!<Frame Format                        */
#define  SPI_CR1_SSI                         ((uint16_t)0x0100)            /*!<Internal slave select               */
#define  SPI_CR1_SSM                         ((uint16_t)0x0200)            /*!<Software slave management           */
#define  SPI_CR1_RXONLY                      ((uint16_t)0x0400)            /*!<Receive only                        */
#define  SPI_CR1_DFF                         ((uint16_t)0x0800)            /*!<Data Frame Format                   */
#define  SPI_CR1_CRCNEXT                     ((uint16_t)0x1000)            /*!<Transmit CRC next                   */
#define  SPI_CR1_CRCEN                       ((uint16_t)0x2000)            /*!<Hardware CRC calculation enable     */
#define  SPI_CR1_BIDIOE                      ((uint16_t)0x4000)            /*!<Output enable in bidirectional mode */
#define  SPI_CR1_BIDIMODE                    ((uint16_t)0x8000)            /*!<Bidirectional data mode enable      */

/*******************  Bit definition for SPI_CR2 register  ********************/
#define  SPI_CR2_RXDMAEN                     ((uint8_t)0x01)               /*!<Rx Buffer DMA Enable                 */
#define  SPI_CR2_TXDMAEN                     ((uint8_t)0x02)               /*!<Tx Buffer DMA Enable                 */
#define  SPI_CR2_SSOE                        ((uint8_t)0x04)               /*!<SS Output Enable                     */
#define  SPI_CR2_ERRIE                       ((uint8_t)0x20)               /*!<Error Interrupt Enable               */
#define  SPI_CR2_RXNEIE                      ((uint8_t)0x40)               /*!<RX buffer Not Empty Interrupt Enable */
#define  SPI_CR2_TXEIE                       ((uint8_t)0x80)               /*!<Tx buffer Empty Interrupt Enable     */

/********************  Bit definition for SPI_SR register  ********************/
#define  SPI_SR_RXNE                         ((uint8_t)0x01)               /*!<Receive buffer Not Empty */
#define  SPI_SR_TXE                          ((uint8_t)0x02)               /*!<Transmit buffer Empty    */
#define  SPI_SR_CHSIDE                       ((uint8_t)0x04)               /*!<Channel side             */
#define  SPI_SR_UDR                          ((uint8_t)0x08)               /*!<Underrun flag            */
#define  SPI_SR_CRCERR                       ((uint8_t)0x10)               /*!<CRC Error flag           */
#define  SPI_SR_MODF                         ((uint8_t)0x20)               /*!<Mode fault               */
#define  SPI_SR_OVR                          ((uint8_t)0x40)               /*!<Overrun flag             */
#define  SPI_SR_BSY                          ((uint8_t)0x80)               /*!<Busy flag                */

/********************  Bit definition for SPI_DR register  ********************/
#define  SPI_DR_DR                           ((uint16_t)0xFFFF)            /*!<Data Register           */

/*******************  Bit definition for SPI_CRCPR register  ******************/
#define  SPI_CRCPR_CRCPOLY                   ((uint16_t)0xFFFF)            /*!<CRC polynomial register */

/******************  Bit definition for SPI_RXCRCR register  ******************/
#define  SPI_RXCRCR_RXCRC                    ((uint16_t)0xFFFF)            /*!<Rx CRC Register         */

/******************  Bit definition for SPI_TXCRCR register  ******************/
#define  SPI_TXCRCR_TXCRC                    ((uint16_t)0xFFFF)            /*!<Tx CRC Register         */

/******************  Bit definition for SPI_I2SCFGR register  *****************/
#define  SPI_I2SCFGR_CHLEN                   ((uint16_t)0x0001)            /*!<Channel length (number of bits per audio channel) */

#define  SPI_I2SCFGR_DATLEN                  ((uint16_t)0x0006)            /*!<DATLEN[1:0] bits (Data length to be transferred)  */
#define  SPI_I2SCFGR_DATLEN_0                ((uint16_t)0x0002)            /*!<Bit 0 */
#define  SPI_I2SCFGR_DATLEN_1                ((uint16_t)0x0004)            /*!<Bit 1 */

#define  SPI_I2SCFGR_CKPOL                   ((uint16_t)0x0008)            /*!<steady state clock polarity               */

#define  SPI_I2SCFGR_I2SSTD                  ((uint16_t)0x0030)            /*!<I2SSTD[1:0] bits (I2S standard selection) */
#define  SPI_I2SCFGR_I2SSTD_0                ((uint16_t)0x0010)            /*!<Bit 0 */
#define  SPI_I2SCFGR_I2SSTD_1                ((uint16_t)0x0020)            /*!<Bit 1 */

#define  SPI_I2SCFGR_PCMSYNC                 ((uint16_t)0x0080)            /*!<PCM frame synchronization                 */

#define  SPI_I2SCFGR_I2SCFG                  ((uint16_t)0x0300)            /*!<I2SCFG[1:0] bits (I2S configuration mode) */
#define  SPI_I2SCFGR_I2SCFG_0                ((uint16_t)0x0100)            /*!<Bit 0 */
#define  SPI_I2SCFGR_I2SCFG_1                ((uint16_t)0x0200)            /*!<Bit 1 */

#define  SPI_I2SCFGR_I2SE                    ((uint16_t)0x0400)            /*!<I2S Enable         */
#define  SPI_I2SCFGR_I2SMOD                  ((uint16_t)0x0800)            /*!<I2S mode selection */

/******************  Bit definition for SPI_I2SPR register  *******************/
#define  SPI_I2SPR_I2SDIV                    ((uint16_t)0x00FF)            /*!<I2S Linear prescaler         */
#define  SPI_I2SPR_ODD                       ((uint16_t)0x0100)            /*!<Odd factor for the prescaler */
#define  SPI_I2SPR_MCKOE                     ((uint16_t)0x0200)            /*!<Master Clock Output Enable   */

#endif