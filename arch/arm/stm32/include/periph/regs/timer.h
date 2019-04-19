#ifndef STM32_PERIPH_TIMER_H
#define STM32_PERIPH_TIMER_H

struct timer_regs {
	volatile uint16_t CR1;         /*!< TIM control register 1,              Address offset: 0x00 */
	uint16_t      RESERVED0;   /*!< Reserved, 0x02                                            */
	volatile uint16_t CR2;         /*!< TIM control register 2,              Address offset: 0x04 */
	uint16_t      RESERVED1;   /*!< Reserved, 0x06                                            */
	volatile uint16_t SMCR;        /*!< TIM slave mode control register,     Address offset: 0x08 */
	uint16_t      RESERVED2;   /*!< Reserved, 0x0A                                            */
	volatile uint16_t DIER;        /*!< TIM DMA/interrupt enable register,   Address offset: 0x0C */
	uint16_t      RESERVED3;   /*!< Reserved, 0x0E                                            */
	volatile uint16_t SR;          /*!< TIM status register,                 Address offset: 0x10 */
	uint16_t      RESERVED4;   /*!< Reserved, 0x12                                            */
	volatile uint16_t EGR;         /*!< TIM event generation register,       Address offset: 0x14 */
	uint16_t      RESERVED5;   /*!< Reserved, 0x16                                            */
	volatile uint16_t CCMR1;       /*!< TIM capture/compare mode register 1, Address offset: 0x18 */
	uint16_t      RESERVED6;   /*!< Reserved, 0x1A                                            */
	volatile uint16_t CCMR2;       /*!< TIM capture/compare mode register 2, Address offset: 0x1C */
	uint16_t      RESERVED7;   /*!< Reserved, 0x1E                                            */
	volatile uint16_t CCER;        /*!< TIM capture/compare enable register, Address offset: 0x20 */
	uint16_t      RESERVED8;   /*!< Reserved, 0x22                                            */
	volatile uint32_t CNT;         /*!< TIM counter register,                Address offset: 0x24 */
	volatile uint16_t PSC;         /*!< TIM prescaler,                       Address offset: 0x28 */
	uint16_t      RESERVED9;   /*!< Reserved, 0x2A                                            */
	volatile uint32_t ARR;         /*!< TIM auto-reload register,            Address offset: 0x2C */
	volatile uint16_t RCR;         /*!< TIM repetition counter register,     Address offset: 0x30 */
	uint16_t      RESERVED10;  /*!< Reserved, 0x32                                            */
	volatile uint32_t CCR1;        /*!< TIM capture/compare register 1,      Address offset: 0x34 */
	volatile uint32_t CCR2;        /*!< TIM capture/compare register 2,      Address offset: 0x38 */
	volatile uint32_t CCR3;        /*!< TIM capture/compare register 3,      Address offset: 0x3C */
	volatile uint32_t CCR4;        /*!< TIM capture/compare register 4,      Address offset: 0x40 */
	volatile uint16_t BDTR;        /*!< TIM break and dead-time register,    Address offset: 0x44 */
	uint16_t      RESERVED11;  /*!< Reserved, 0x46                                            */
	volatile uint16_t DCR;         /*!< TIM DMA control register,            Address offset: 0x48 */
	uint16_t      RESERVED12;  /*!< Reserved, 0x4A                                            */
	volatile uint16_t DMAR;        /*!< TIM DMA address for full transfer,   Address offset: 0x4C */
	uint16_t      RESERVED13;  /*!< Reserved, 0x4E                                            */
	volatile uint16_t OR;          /*!< TIM option register,                 Address offset: 0x50 */
	uint16_t      RESERVED14;  /*!< Reserved, 0x52                                            */
};

#endif