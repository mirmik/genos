#ifndef STM32_USART_REGS_H
#define STM32_USART_REGS_H

#include <inttypes.h>

struct usart_regs {
  volatile uint16_t SR;         /*!< USART Status register,                   Address offset: 0x00 */
  uint16_t      RESERVED0;  /*!< Reserved, 0x02                                                */
  volatile uint16_t DR;         /*!< USART Data register,                     Address offset: 0x04 */
  uint16_t      RESERVED1;  /*!< Reserved, 0x06                                                */
  volatile uint16_t BRR;        /*!< USART Baud rate register,                Address offset: 0x08 */
  uint16_t      RESERVED2;  /*!< Reserved, 0x0A                                                */
  volatile uint16_t CR1;        /*!< USART Control register 1,                Address offset: 0x0C */
  uint16_t      RESERVED3;  /*!< Reserved, 0x0E                                                */
  volatile uint16_t CR2;        /*!< USART Control register 2,                Address offset: 0x10 */
  uint16_t      RESERVED4;  /*!< Reserved, 0x12                                                */
  volatile uint16_t CR3;        /*!< USART Control register 3,                Address offset: 0x14 */
  uint16_t      RESERVED5;  /*!< Reserved, 0x16                                                */
  volatile uint16_t GTPR;       /*!< USART Guard time and prescaler register, Address offset: 0x18 */
  uint16_t      RESERVED6;  /*!< Reserved, 0x1A                                                */
};

struct usart_irqno {
  uint8_t rx_irqno;
  uint8_t tx_irqno;
  uint8_t tx_complete_irqno;
};

/******************************************************************************/
/*                                                                            */
/*         Universal Synchronous Asynchronous Receiver Transmitter            */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for USART_SR register  *******************/
#define  USART_SR_PE                         ((uint16_t)0x0001)            /*!<Parity Error                 */
#define  USART_SR_FE                         ((uint16_t)0x0002)            /*!<Framing Error                */
#define  USART_SR_NE                         ((uint16_t)0x0004)            /*!<Noise Error Flag             */
#define  USART_SR_ORE                        ((uint16_t)0x0008)            /*!<OverRun Error                */
#define  USART_SR_IDLE                       ((uint16_t)0x0010)            /*!<IDLE line detected           */
#define  USART_SR_RXNE                       ((uint16_t)0x0020)            /*!<Read Data Register Not Empty */
#define  USART_SR_TC                         ((uint16_t)0x0040)            /*!<Transmission Complete        */
#define  USART_SR_TXE                        ((uint16_t)0x0080)            /*!<Transmit Data Register Empty */
#define  USART_SR_LBD                        ((uint16_t)0x0100)            /*!<LIN Break Detection Flag     */
#define  USART_SR_CTS                        ((uint16_t)0x0200)            /*!<CTS Flag                     */

/*******************  Bit definition for USART_DR register  *******************/
#define  USART_DR_DR                         ((uint16_t)0x01FF)            /*!<Data value */

/******************  Bit definition for USART_BRR register  *******************/
#define  USART_BRR_DIV_Fraction              ((uint16_t)0x000F)            /*!<Fraction of USARTDIV */
#define  USART_BRR_DIV_Mantissa              ((uint16_t)0xFFF0)            /*!<Mantissa of USARTDIV */

/******************  Bit definition for USART_CR1 register  *******************/
#define  USART_CR1_SBK                       ((uint16_t)0x0001)            /*!<Send Break                             */
#define  USART_CR1_RWU                       ((uint16_t)0x0002)            /*!<Receiver wakeup                        */
#define  USART_CR1_RE                        ((uint16_t)0x0004)            /*!<Receiver Enable                        */
#define  USART_CR1_TE                        ((uint16_t)0x0008)            /*!<Transmitter Enable                     */
#define  USART_CR1_IDLEIE                    ((uint16_t)0x0010)            /*!<IDLE Interrupt Enable                  */
#define  USART_CR1_RXNEIE                    ((uint16_t)0x0020)            /*!<RXNE Interrupt Enable                  */
#define  USART_CR1_TCIE                      ((uint16_t)0x0040)            /*!<Transmission Complete Interrupt Enable */
#define  USART_CR1_TXEIE                     ((uint16_t)0x0080)            /*!<PE Interrupt Enable                    */
#define  USART_CR1_PEIE                      ((uint16_t)0x0100)            /*!<PE Interrupt Enable                    */
#define  USART_CR1_PS                        ((uint16_t)0x0200)            /*!<Parity Selection                       */
#define  USART_CR1_PCE                       ((uint16_t)0x0400)            /*!<Parity Control Enable                  */
#define  USART_CR1_WAKE                      ((uint16_t)0x0800)            /*!<Wakeup method                          */
#define  USART_CR1_M                         ((uint16_t)0x1000)            /*!<Word length                            */
#define  USART_CR1_UE                        ((uint16_t)0x2000)            /*!<USART Enable                           */
#define  USART_CR1_OVER8                     ((uint16_t)0x8000)            /*!<USART Oversampling by 8 enable         */

/******************  Bit definition for USART_CR2 register  *******************/
#define  USART_CR2_ADD                       ((uint16_t)0x000F)            /*!<Address of the USART node            */
#define  USART_CR2_LBDL                      ((uint16_t)0x0020)            /*!<LIN Break Detection Length           */
#define  USART_CR2_LBDIE                     ((uint16_t)0x0040)            /*!<LIN Break Detection Interrupt Enable */
#define  USART_CR2_LBCL                      ((uint16_t)0x0100)            /*!<Last Bit Clock pulse                 */
#define  USART_CR2_CPHA                      ((uint16_t)0x0200)            /*!<Clock Phase                          */
#define  USART_CR2_CPOL                      ((uint16_t)0x0400)            /*!<Clock Polarity                       */
#define  USART_CR2_CLKEN                     ((uint16_t)0x0800)            /*!<Clock Enable                         */

#define  USART_CR2_STOP                      ((uint16_t)0x3000)            /*!<STOP[1:0] bits (STOP bits) */
#define  USART_CR2_STOP_0                    ((uint16_t)0x1000)            /*!<Bit 0 */
#define  USART_CR2_STOP_1                    ((uint16_t)0x2000)            /*!<Bit 1 */

#define  USART_CR2_LINEN                     ((uint16_t)0x4000)            /*!<LIN mode enable */

/******************  Bit definition for USART_CR3 register  *******************/
#define  USART_CR3_EIE                       ((uint16_t)0x0001)            /*!<Error Interrupt Enable      */
#define  USART_CR3_IREN                      ((uint16_t)0x0002)            /*!<IrDA mode Enable            */
#define  USART_CR3_IRLP                      ((uint16_t)0x0004)            /*!<IrDA Low-Power              */
#define  USART_CR3_HDSEL                     ((uint16_t)0x0008)            /*!<Half-Duplex Selection       */
#define  USART_CR3_NACK                      ((uint16_t)0x0010)            /*!<Smartcard NACK enable       */
#define  USART_CR3_SCEN                      ((uint16_t)0x0020)            /*!<Smartcard mode enable       */
#define  USART_CR3_DMAR                      ((uint16_t)0x0040)            /*!<DMA Enable Receiver         */
#define  USART_CR3_DMAT                      ((uint16_t)0x0080)            /*!<DMA Enable Transmitter      */
#define  USART_CR3_RTSE                      ((uint16_t)0x0100)            /*!<RTS Enable                  */
#define  USART_CR3_CTSE                      ((uint16_t)0x0200)            /*!<CTS Enable                  */
#define  USART_CR3_CTSIE                     ((uint16_t)0x0400)            /*!<CTS Interrupt Enable        */
#define  USART_CR3_ONEBIT                    ((uint16_t)0x0800)            /*!<USART One bit method enable */

/******************  Bit definition for USART_GTPR register  ******************/
#define  USART_GTPR_PSC                      ((uint16_t)0x00FF)            /*!<PSC[7:0] bits (Prescaler value) */
#define  USART_GTPR_PSC_0                    ((uint16_t)0x0001)            /*!<Bit 0 */
#define  USART_GTPR_PSC_1                    ((uint16_t)0x0002)            /*!<Bit 1 */
#define  USART_GTPR_PSC_2                    ((uint16_t)0x0004)            /*!<Bit 2 */
#define  USART_GTPR_PSC_3                    ((uint16_t)0x0008)            /*!<Bit 3 */
#define  USART_GTPR_PSC_4                    ((uint16_t)0x0010)            /*!<Bit 4 */
#define  USART_GTPR_PSC_5                    ((uint16_t)0x0020)            /*!<Bit 5 */
#define  USART_GTPR_PSC_6                    ((uint16_t)0x0040)            /*!<Bit 6 */
#define  USART_GTPR_PSC_7                    ((uint16_t)0x0080)            /*!<Bit 7 */

#define  USART_GTPR_GT                       ((uint16_t)0xFF00)            /*!<Guard time value */


#endif