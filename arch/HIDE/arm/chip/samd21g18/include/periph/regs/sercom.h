#ifndef GENOS_SERCOM_REGS_H
#define GENOS_SERCOM_REGS_H

#include <genos/util/regs.h>
#include <inttypes.h>

typedef union {
	struct {
		uint32_t SWRST:1;          /*!< bit:      0  Software Reset                     */
		uint32_t ENABLE:1;         /*!< bit:      1  Enable                             */
		uint32_t MODE:3;           /*!< bit:  2.. 4  Operating Mode                     */
		uint32_t :2;               /*!< bit:  5.. 6  Reserved                           */
		uint32_t RUNSTDBY:1;       /*!< bit:      7  Run during Standby                 */
		uint32_t IBON:1;           /*!< bit:      8  Immediate Buffer Overflow Notification */
		uint32_t :4;               /*!< bit:  9..12  Reserved                           */
		uint32_t SAMPR:3;          /*!< bit: 13..15  Sample                             */
		uint32_t TXPO:2;           /*!< bit: 16..17  Transmit Data Pinout               */
		uint32_t :2;               /*!< bit: 18..19  Reserved                           */
		uint32_t RXPO:2;           /*!< bit: 20..21  Receive Data Pinout                */
		uint32_t SAMPA:2;          /*!< bit: 22..23  Sample Adjustment                  */
		uint32_t FORM:4;           /*!< bit: 24..27  Frame Format                       */
		uint32_t CMODE:1;          /*!< bit:     28  Communication Mode                 */
		uint32_t CPOL:1;           /*!< bit:     29  Clock Polarity                     */
		uint32_t DORD:1;           /*!< bit:     30  Data Order                         */
		uint32_t :1;               /*!< bit:     31  Reserved                           */
	} bit;                       /*!< Structure used for bit  access                  */
	uint32_t reg;                /*!< Type      used for register access              */
} SERCOM_USART_CTRLA_Type;

typedef union {
	struct {
		uint32_t CHSIZE:3;         /*!< bit:  0.. 2  Character Size                     */
		uint32_t :3;               /*!< bit:  3.. 5  Reserved                           */
		uint32_t SBMODE:1;         /*!< bit:      6  Stop Bit Mode                      */
		uint32_t :1;               /*!< bit:      7  Reserved                           */
		uint32_t COLDEN:1;         /*!< bit:      8  Collision Detection Enable         */
		uint32_t SFDE:1;           /*!< bit:      9  Start of Frame Detection Enable    */
		uint32_t ENC:1;            /*!< bit:     10  Encoding Format                    */
		uint32_t :2;               /*!< bit: 11..12  Reserved                           */
		uint32_t PMODE:1;          /*!< bit:     13  Parity Mode                        */
		uint32_t :2;               /*!< bit: 14..15  Reserved                           */
		uint32_t TXEN:1;           /*!< bit:     16  Transmitter Enable                 */
		uint32_t RXEN:1;           /*!< bit:     17  Receiver Enable                    */
		uint32_t :14;              /*!< bit: 18..31  Reserved                           */
	} bit;                       /*!< Structure used for bit  access                  */
	uint32_t reg;                /*!< Type      used for register access              */
} SERCOM_USART_CTRLB_Type;

typedef union {
	struct {
		uint16_t BAUD:16;          /*!< bit:  0..15  Baud Rate Value                    */
	} bit;                       /*!< Structure used for bit  access                  */
	struct { // FRAC mode
		uint16_t BAUD:13;          /*!< bit:  0..12  Baud Rate Value                    */
		uint16_t FP:3;             /*!< bit: 13..15  Fractional Part                    */
	} FRAC;                      /*!< Structure used for FRAC                         */
	struct { // FRACFP mode
		uint16_t BAUD:13;          /*!< bit:  0..12  Baud Rate Value                    */
		uint16_t FP:3;             /*!< bit: 13..15  Fractional Part                    */
	} FRACFP;                    /*!< Structure used for FRACFP                       */
	struct { // USARTFP mode
		uint16_t BAUD:16;          /*!< bit:  0..15  Baud Rate Value                    */
	} USARTFP;                   /*!< Structure used for USARTFP                      */
	uint16_t reg;                /*!< Type      used for register access              */
} SERCOM_USART_BAUD_Type;

typedef union {
	struct {
		uint8_t  RXPL:8;           /*!< bit:  0.. 7  Receive Pulse Length               */
	} bit;                       /*!< Structure used for bit  access                  */
	uint8_t reg;                 /*!< Type      used for register access              */
} SERCOM_USART_RXPL_Type;

typedef union {
	struct {
		uint8_t  DRE:1;            /*!< bit:      0  Data Register Empty Interrupt Disable */
		uint8_t  TXC:1;            /*!< bit:      1  Transmit Complete Interrupt Disable */
		uint8_t  RXC:1;            /*!< bit:      2  Receive Complete Interrupt Disable */
		uint8_t  RXS:1;            /*!< bit:      3  Receive Start Interrupt Disable    */
		uint8_t  CTSIC:1;          /*!< bit:      4  Clear To Send Input Change Interrupt Disable */
		uint8_t  RXBRK:1;          /*!< bit:      5  Break Received Interrupt Disable   */
		uint8_t  :1;               /*!< bit:      6  Reserved                           */
		uint8_t  ERROR:1;          /*!< bit:      7  Combined Error Interrupt Disable   */
	} bit;                       /*!< Structure used for bit  access                  */
	uint8_t reg;                 /*!< Type      used for register access              */
} SERCOM_USART_INTENCLR_Type;

typedef union {
	struct {
		uint8_t  DRE:1;            /*!< bit:      0  Data Register Empty Interrupt Enable */
		uint8_t  TXC:1;            /*!< bit:      1  Transmit Complete Interrupt Enable */
		uint8_t  RXC:1;            /*!< bit:      2  Receive Complete Interrupt Enable  */
		uint8_t  RXS:1;            /*!< bit:      3  Receive Start Interrupt Enable     */
		uint8_t  CTSIC:1;          /*!< bit:      4  Clear To Send Input Change Interrupt Enable */
		uint8_t  RXBRK:1;          /*!< bit:      5  Break Received Interrupt Enable    */
		uint8_t  :1;               /*!< bit:      6  Reserved                           */
		uint8_t  ERROR:1;          /*!< bit:      7  Combined Error Interrupt Enable    */
	} bit;                       /*!< Structure used for bit  access                  */
	uint8_t reg;                 /*!< Type      used for register access              */
} SERCOM_USART_INTENSET_Type;

typedef union { // __I to avoid read-modify-write on write-to-clear register
	struct {
		__I uint8_t  DRE:1;            /*!< bit:      0  Data Register Empty Interrupt      */
		__I uint8_t  TXC:1;            /*!< bit:      1  Transmit Complete Interrupt        */
		__I uint8_t  RXC:1;            /*!< bit:      2  Receive Complete Interrupt         */
		__I uint8_t  RXS:1;            /*!< bit:      3  Receive Start Interrupt            */
		__I uint8_t  CTSIC:1;          /*!< bit:      4  Clear To Send Input Change Interrupt */
		__I uint8_t  RXBRK:1;          /*!< bit:      5  Break Received Interrupt           */
		__I uint8_t  :1;               /*!< bit:      6  Reserved                           */
		__I uint8_t  ERROR:1;          /*!< bit:      7  Combined Error Interrupt           */
	} bit;                       /*!< Structure used for bit  access                  */
	uint8_t reg;                 /*!< Type      used for register access              */
} SERCOM_USART_INTFLAG_Type;

typedef union {
	struct {
		uint16_t PERR:1;           /*!< bit:      0  Parity Error                       */
		uint16_t FERR:1;           /*!< bit:      1  Frame Error                        */
		uint16_t BUFOVF:1;         /*!< bit:      2  Buffer Overflow                    */
		uint16_t CTS:1;            /*!< bit:      3  Clear To Send                      */
		uint16_t ISF:1;            /*!< bit:      4  Inconsistent Sync Field            */
		uint16_t COLL:1;           /*!< bit:      5  Collision Detected                 */
		uint16_t :10;              /*!< bit:  6..15  Reserved                           */
	} bit;                       /*!< Structure used for bit  access                  */
	uint16_t reg;                /*!< Type      used for register access              */
} SERCOM_USART_STATUS_Type;

typedef union {
	struct {
		uint32_t SWRST:1;          /*!< bit:      0  Software Reset Synchronization Busy */
		uint32_t ENABLE:1;         /*!< bit:      1  SERCOM Enable Synchronization Busy */
		uint32_t CTRLB:1;          /*!< bit:      2  CTRLB Synchronization Busy         */
		uint32_t :29;              /*!< bit:  3..31  Reserved                           */
	} bit;                       /*!< Structure used for bit  access                  */
	uint32_t reg;                /*!< Type      used for register access              */
} SERCOM_USART_SYNCBUSY_Type;

typedef union {
	struct {
		uint16_t DATA:9;           /*!< bit:  0.. 8  Data Value                         */
		uint16_t :7;               /*!< bit:  9..15  Reserved                           */
	} bit;                       /*!< Structure used for bit  access                  */
	uint16_t reg;                /*!< Type      used for register access              */
} SERCOM_USART_DATA_Type;

typedef union {
	struct {
		uint8_t  DBGSTOP:1;        /*!< bit:      0  Debug Mode                         */
		uint8_t  :7;               /*!< bit:  1.. 7  Reserved                           */
	} bit;                       /*!< Structure used for bit  access                  */
	uint8_t reg;                 /*!< Type      used for register access              */
} SERCOM_USART_DBGCTRL_Type;

//typedef struct { /* I2C Master Mode */
//  __IO SERCOM_I2CM_CTRLA_Type    CTRLA;       /**< \brief Offset: 0x00 (R/W 32) I2CM Control A */
//  __IO SERCOM_I2CM_CTRLB_Type    CTRLB;       /**< \brief Offset: 0x04 (R/W 32) I2CM Control B */
//	   uint8_t                    Reserved1[0x4];
//  __IO SERCOM_I2CM_BAUD_Type     BAUD;        /**< \brief Offset: 0x0C (R/W 32) I2CM Baud Rate */
//	   uint8_t                    Reserved2[0x4];
//  __IO SERCOM_I2CM_INTENCLR_Type INTENCLR;    /**< \brief Offset: 0x14 (R/W  8) I2CM Interrupt Enable Clear */
//	   uint8_t                    Reserved3[0x1];
//  __IO SERCOM_I2CM_INTENSET_Type INTENSET;    /**< \brief Offset: 0x16 (R/W  8) I2CM Interrupt Enable Set */
//	   uint8_t                    Reserved4[0x1];
//  __IO SERCOM_I2CM_INTFLAG_Type  INTFLAG;     /**< \brief Offset: 0x18 (R/W  8) I2CM Interrupt Flag Status and Clear */
//	   uint8_t                    Reserved5[0x1];
//  __IO SERCOM_I2CM_STATUS_Type   STATUS;      /**< \brief Offset: 0x1A (R/W 16) I2CM Status */
//  __I  SERCOM_I2CM_SYNCBUSY_Type SYNCBUSY;    /**< \brief Offset: 0x1C (R/  32) I2CM Syncbusy */
//	   uint8_t                    Reserved6[0x4];
//  __IO SERCOM_I2CM_ADDR_Type     ADDR;        /**< \brief Offset: 0x24 (R/W 32) I2CM Address */
//  __IO SERCOM_I2CM_DATA_Type     DATA;        /**< \brief Offset: 0x28 (R/W  8) I2CM Data */
//	   uint8_t                    Reserved7[0x7];
//  __IO SERCOM_I2CM_DBGCTRL_Type  DBGCTRL;     /**< \brief Offset: 0x30 (R/W  8) I2CM Debug Control */
//} SercomI2cm;
//
//typedef struct { /* I2C Slave Mode */
//  __IO SERCOM_I2CS_CTRLA_Type    CTRLA;       /**< \brief Offset: 0x00 (R/W 32) I2CS Control A */
//  __IO SERCOM_I2CS_CTRLB_Type    CTRLB;       /**< \brief Offset: 0x04 (R/W 32) I2CS Control B */
//	   uint8_t                    Reserved1[0xC];
//  __IO SERCOM_I2CS_INTENCLR_Type INTENCLR;    /**< \brief Offset: 0x14 (R/W  8) I2CS Interrupt Enable Clear */
//	   uint8_t                    Reserved2[0x1];
//  __IO SERCOM_I2CS_INTENSET_Type INTENSET;    /**< \brief Offset: 0x16 (R/W  8) I2CS Interrupt Enable Set */
//	   uint8_t                    Reserved3[0x1];
//  __IO SERCOM_I2CS_INTFLAG_Type  INTFLAG;     /**< \brief Offset: 0x18 (R/W  8) I2CS Interrupt Flag Status and Clear */
//	   uint8_t                    Reserved4[0x1];
//  __IO SERCOM_I2CS_STATUS_Type   STATUS;      /**< \brief Offset: 0x1A (R/W 16) I2CS Status */
//  __I  SERCOM_I2CS_SYNCBUSY_Type SYNCBUSY;    /**< \brief Offset: 0x1C (R/  32) I2CS Syncbusy */
//	   uint8_t                    Reserved5[0x4];
//  __IO SERCOM_I2CS_ADDR_Type     ADDR;        /**< \brief Offset: 0x24 (R/W 32) I2CS Address */
//  __IO SERCOM_I2CS_DATA_Type     DATA;        /**< \brief Offset: 0x28 (R/W  8) I2CS Data */
//} SercomI2cs;
//
//typedef struct { /* SPI Mode */
//  __IO SERCOM_SPI_CTRLA_Type     CTRLA;       /**< \brief Offset: 0x00 (R/W 32) SPI Control A */
//  __IO SERCOM_SPI_CTRLB_Type     CTRLB;       /**< \brief Offset: 0x04 (R/W 32) SPI Control B */
//	   uint8_t                    Reserved1[0x4];
//  __IO SERCOM_SPI_BAUD_Type      BAUD;        /**< \brief Offset: 0x0C (R/W  8) SPI Baud Rate */
//	   uint8_t                    Reserved2[0x7];
//  __IO SERCOM_SPI_INTENCLR_Type  INTENCLR;    /**< \brief Offset: 0x14 (R/W  8) SPI Interrupt Enable Clear */
//	   uint8_t                    Reserved3[0x1];
//  __IO SERCOM_SPI_INTENSET_Type  INTENSET;    /**< \brief Offset: 0x16 (R/W  8) SPI Interrupt Enable Set */
//	   uint8_t                    Reserved4[0x1];
//  __IO SERCOM_SPI_INTFLAG_Type   INTFLAG;     /**< \brief Offset: 0x18 (R/W  8) SPI Interrupt Flag Status and Clear */
//	   uint8_t                    Reserved5[0x1];
//  __IO SERCOM_SPI_STATUS_Type    STATUS;      /**< \brief Offset: 0x1A (R/W 16) SPI Status */
//  __I  SERCOM_SPI_SYNCBUSY_Type  SYNCBUSY;    /**< \brief Offset: 0x1C (R/  32) SPI Syncbusy */
//	   uint8_t                    Reserved6[0x4];
//  __IO SERCOM_SPI_ADDR_Type      ADDR;        /**< \brief Offset: 0x24 (R/W 32) SPI Address */
//  __IO SERCOM_SPI_DATA_Type      DATA;        /**< \brief Offset: 0x28 (R/W 32) SPI Data */
//	   uint8_t                    Reserved7[0x4];
//  __IO SERCOM_SPI_DBGCTRL_Type   DBGCTRL;     /**< \brief Offset: 0x30 (R/W  8) SPI Debug Control */
//} SercomSpi;

typedef struct { /* USART Mode */
	__IO SERCOM_USART_CTRLA_Type   CTRLA;       		//0x00 (R/W 32) USART Control A
	__IO SERCOM_USART_CTRLB_Type   CTRLB;       		//0x04 (R/W 32) USART Control B
			 uint8_t Reserved1[0x4];
	__IO SERCOM_USART_BAUD_Type    BAUD;        		//0x0C (R/W 16) USART Baud Rate
	__IO SERCOM_USART_RXPL_Type    RXPL;        		//0x0E (R/W  8) USART Receive Pulse Length
			 uint8_t Reserved2[0x5];
	__IO SERCOM_USART_INTENCLR_Type INTENCLR;    		//0x14 (R/W  8) USART Interrupt Enable Clear
			 uint8_t Reserved3[0x1];
	__IO SERCOM_USART_INTENSET_Type INTENSET;    		//0x16 (R/W  8) USART Interrupt Enable Set
			 uint8_t Reserved4[0x1];
	__IO SERCOM_USART_INTFLAG_Type INTFLAG;     		//0x18 (R/W  8) USART Interrupt Flag Status and Clear
			 uint8_t Reserved5[0x1];
	__IO SERCOM_USART_STATUS_Type  STATUS;      		//0x1A (R/W 16) USART Status
	__I  SERCOM_USART_SYNCBUSY_Type SYNCBUSY;    		//0x1C (R/  32) USART Syncbusy
			 uint8_t Reserved6[0x8];
	__IO SERCOM_USART_DATA_Type    DATA;        		//0x28 (R/W 16) USART Data
			 uint8_t Reserved7[0x6];
	__IO SERCOM_USART_DBGCTRL_Type DBGCTRL;     		//0x30 (R/W  8) USART Debug Control
} SercomUsart;

typedef union {
	//SercomI2cm  I2CM;        // 0x00 I2C Master Mode
	//SercomI2cs  I2CS;        // 0x00 I2C Slave Mode
	//SercomSpi   SPI;         // 0x00 SPI Mode
	SercomUsart USART;       // 0x00 USART Mode
} sercom_regs;

#endif