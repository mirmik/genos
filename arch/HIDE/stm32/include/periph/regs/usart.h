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

/******************  Bit definition for USART_CR1 register  *******************/
#define  USART_CR1_SBK                       ((uint16_t)0x0001)            /*!<Send Break                             */
#define  USART_CR1_RWU                       ((uint16_t)0x0002)            /*!<Receiver wakeup                        */
#define  USART_CR1_RE                        ((uint16_t)0x0004)            /*!<Receiver Enable                        */
#define  USART_CR1_TE                        ((uint16_t)0x0008)            /*!<Transmitter Enable                     */
#define  USART_CR1_IDLEIE                    ((uint16_t)0x0010)            /*!<IDLE Interrupt Enable                  */
#define  USART_CR1_RXNEIE                    ((uint16_t)0x0020)            /*!<RXNE Interrupt Enable                  */
#define  USART_CR1_TCIE                      ((uint16_t)0x0040)            /*!<Transmission Complete Interrupt Enable */
#define  USART_CR1_TXEIE                     ((uint16_t)0x0080)            /*!<TXE Interrupt Enable                    */
#define  USART_CR1_PEIE                      ((uint16_t)0x0100)            /*!<PE Interrupt Enable                    */
#define  USART_CR1_PS                        ((uint16_t)0x0200)            /*!<Parity Selection                       */
#define  USART_CR1_PCE                       ((uint16_t)0x0400)            /*!<Parity Control Enable                  */
#define  USART_CR1_WAKE                      ((uint16_t)0x0800)            /*!<Wakeup method                          */
#define  USART_CR1_M                         ((uint16_t)0x1000)            /*!<Word length                            */
#define  USART_CR1_UE                        ((uint16_t)0x2000)            /*!<USART Enable                           */
#define  USART_CR1_OVER8                     ((uint16_t)0x8000)            /*!<USART Oversampling by 8 enable         */

#endif