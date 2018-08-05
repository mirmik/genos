#include <avr/io.h>
#include <avr/common.h>
#include <avr/sfr_defs.h>

/****************************************************************************
 * External Symbols
 ****************************************************************************/

	.global	vectortab

#if defined(CHIP_ATMEGA128)
	.global	atmega_int0			/* External interrupt request 0 */
	.global	atmega_int1			/* External interrupt request 1 */
	.global	atmega_int2			/* External interrupt request 2 */
	.global	atmega_int3			/* External interrupt request 3 */
	.global	atmega_int4			/* External interrupt request 4 */
	.global	atmega_int5			/* External interrupt request 5 */
	.global	atmega_int6			/* External interrupt request 6 */
	.global	atmega_int7			/* External interrupt request 7 */
	.global	atmega_t2comp		/* TIMER2 COMP timer/counter2 compare match */
	.global	atmega_t2ovf		/* TIMER2 OVF timer/counter2 overflow */
	.global	atmega_t1capt		/* TIMER1 CAPT timer/counter1 capture event */
	.global	atmega_t1compa		/* TIMER1 COMPA timer/counter1 compare match a */
	.global	atmega_t1compb		/* TIMER1 COMPB timer/counter1 compare match b */
	.global	atmega_t1ovf		/* TIMER1 OVF timer/counter1 overflow */
	.global	atmega_t0comp		/* TIMER0 COMP timer/counter0 compare match */
	.global	atmega_t0ovf		/* TIMER0 OVF timer/counter0 overflow */
	.global	atmega_spi			/* STC SPI serial transfer complete */
	.global	atmega_u0rx			/* USART0 RX complete */
	.global	atmega_u0dre		/* USART0 data register empty */
	.global	atmega_u0tx			/* USART0 TX complete */
	.global	atmega_adc			/* ADC conversion complete */
	.global	atmega_ee			/* EEPROM ready */
	.global	atmega_anacomp		/* ANALOG COMP analog comparator */
	.global	atmega_t1compc		/* TIMER1 COMPC timer/countre1 compare match c */
	.global	atmega_t3capt		/* TIMER3 CAPT timer/counter3 capture event */
	.global	atmega_t3compa		/* TIMER3 COMPA timer/counter3 compare match a */
	.global	atmega_t3compb		/* TIMER3 COMPB timer/counter3 compare match b */
	.global	atmega_t3compc		/* TIMER3 COMPC timer/counter3 compare match c */
	.global	atmega_t3ovf		/* TIMER3 OVF timer/counter3 overflow */
	.global	atmega_u1rx			/* USART1 RX complete */
	.global	atmega_u1dre		/* USART1 data register empty */
	.global	atmega_u1tx			/* USART1 TX complete */
	.global	atmega_twi			/* TWI two-wire serial interface */
	.global	atmega_spmrdy		/* Store program memory ready */
#elif defined(CHIP_ATMEGA1284P)
	.global	atmega_int0 		/* External interrupt request 0 */
	.global	atmega_int1 		/* External interrupt request 1 */
	.global	atmega_int2 		/* External interrupt request 2 */
	.global	atmega_pcint0		/* Pin change interrupt request 0 */
	.global	atmega_pcint1		/* Pin change interrupt request 1 */
	.global	atmega_pcint2		/* Pin change interrupt request 2 */
	.global	atmega_pcint3		/* Pin change interrupt request 3 */
	.global	atmega_wdt			/* Watchdog time-out interrupt */
	.global	atmega_t2compa		/* TIMER2 COMPA timer/counter2 compare match */
	.global	atmega_t2compb		/* TIMER2 COMPB timer/counter2 compare match */
	.global	atmega_t2ovf		/* TIMER2 OVF timer/counter2 overflow */
	.global	atmega_t1capt		/* TIMER1 CAPT timer/counter1 capture event */
	.global	atmega_t1compa		/* TIMER1 COMPA timer/counter1 compare match a */
	.global	atmega_t1compb		/* TIMER1 COMPB timer/counter1 compare match b */
	.global	atmega_t1ovf		/* TIMER1 OVF timer/counter1 overflow */
	.global	atmega_t0compa		/* TIMER0 COMPA timer/counter0 compare match */
	.global	atmega_t0compb		/* TIMER0 COMPB timer/counter0 compare match */
	.global	atmega_t0ovf		/* TIMER0 OVF timer/counter0 overflow */
	.global	atmega_spi			/* STC SPI serial transfer complete */
	.global	atmega_u0rx 		/* USART0 RX complete */
	.global	atmega_u0dre		/* USART0 data register empty */
	.global	atmega_u0tx 		/* USART0 TX complete */
	.global	atmega_anacomp		/* ANALOG COMP analog comparator */
	.global	atmega_adc			/* ADC conversion complete */
	.global	atmega_ee 			/* EEPROM ready */
	.global	atmega_twi			/* TWI two-wire serial interface */
	.global	atmega_spmrdy		/* Store program memory ready */
	.global	atmega_u1rx 		/* USART1 RX complete */
	.global	atmega_u1dre		/* USART1 data register empty */
	.global	atmega_u1tx 		/* USART1 TX complete */
	.global	atmega_t3capt		/* TIMER3 CAPT timer/counter3 capture event */
	.global	atmega_t3compa		/* TIMER3 COMPA timer/counter3 compare match a */
	.global	atmega_t3compb		/* TIMER3 COMPB timer/counter3 compare match b */
	.global	atmega_t3ovf		/* TIMER3 OVF timer/counter3 overflow */
#elif defined(CHIP_ATMEGA2560)
	.global	atmega_int0			/* 0x0002 INT0 External Interrupt Request 0 */
	.global	atmega_int1			/* 0x0004 INT1 External Interrupt Request 1 */
	.global	atmega_int2			/* 0x0006 INT2 External Interrupt Request 2 */
	.global	atmega_int3			/* 0x0008 INT3 External Interrupt Request 3 */
	.global	atmega_int4			/* 0x000A INT4 External Interrupt Request 4 */
	.global	atmega_int5			/* 0x000C INT5  External Interrupt Request 5 */
	.global	atmega_int6			/* 0x000E INT6 External Interrupt Request 6 */
	.global	atmega_int7			/* 0x0010 INT7 External Interrupt Request 7 */
	.global	atmega_pcint0		/* 0x0012 PCINT0 Pin Change Interrupt Req 0 */
	.global	atmega_pcint1		/* 0x0014 PCINT1 Pin Change Interrupt Req 1 */
	.global	atmega_pcint2		/* 0x0016 PCINT2 Pin Change Interrupt Req 2 */
	.global	atmega_wdt			/* 0x0018 WDT Watchdog Time-out Interrupt */
	.global	atmega_tim2_compa	/* 0x001A TIMER2 COMPA Timer/Counter2 Compare Match A */
	.global	atmega_tim2_compb	/* 0x001C TIMER2 COMPB Timer/Counter2 Compare Match B */
	.global	atmega_tim2_ovf		/* 0x001E TIMER2 OVF Timer/Counter2 Overflow */
	.global	atmega_tim1_capt	/* 0x0020 TIMER1 CAPT Timer/Counter1 Capture Event */
	.global	atmega_tim1_compa	/* 0x0022 TIMER1 COMPA Timer/Counter1 Compare Match A */
	.global	atmega_tim1_compb	/* 0x0024 TIMER1 COMPB Timer/Counter1 Compare Match B */
	.global	atmega_tim1_compc	/* 0x0026 TIMER1 COMPC Timer/Counter1 Compare Match C */
	.global	atmega_tim1_ovf		/* 0x0028 TIMER1 OVF Timer/Counter1 Overflow */
	.global	atmega_tim0_compa	/* 0x002A TIMER0 COMPA Timer/Counter0 Compare Match A */
	.global	atmega_tim0_compb	/* 0x002C TIMER0 COMPB Timer/Counter0 Compare match B */
	.global	atmega_tim0_ovf		/* 0x002E TIMER0 OVF Timer/Counter0 Overflow */
	.global	atmega_spi_stc		/* 0x0030 SPI, STC SPI Serial Transfer Complete */
	.global	atmega_u0rx			/* 0x0032 USART0 RX USART0 Rx Complete */
	.global	atmega_u0dre		/* 0x0034 USART0 UDRE USART0 Data Register Empty */
	.global	atmega_u0tx			/* 0x0036 USART0 TX USART0 Tx Complete */
	.global	atmega_ana_comp		/* 0x0038 ANALOG COMP Analog Comparator */
	.global	atmega_adc			/* 0x003A ADC ADC Conversion Complete */
	.global	atmega_ee_rdy		/* 0x003C EE READY EEPROM Ready */
	.global	atmega_tim3_capt	/* 0x003E TIMER3 CAPT Timer/Counter3 Capture Event */
	.global	atmega_tim3_compa	/* 0x0040 TIMER3 COMPA Timer/Counter3 Compare Match A */
	.global	atmega_tim3_compb	/* 0x0042 TIMER3 COMPB Timer/Counter3 Compare Match B */
	.global	atmega_tim3_compc	/* 0x0044 TIMER3 COMPC Timer/Counter3 Compare Match C */
	.global	atmega_tim3_ovf		/* 0x0046 TIMER3 OVF Timer/Counter3 Overflow */
	.global	atmega_u1rx			/* 0x0048 USART1 RX USART1 Rx Complete */
	.global	atmega_u1dre		/* 0x004A USART1 UDRE USART1 Data Register Empty */
	.global	atmega_u1tx			/* 0x004C USART1 TX USART1 Tx Complete */
	.global	atmega_twi			/* 0x004E TWI 2-wire Serial Interface */
	.global	atmega_spm_rdy		/* 0x0050 SPM READY Store Program Memory Ready */
	.global	atmega_tim4_capt	/* 0x0052 TIMER4 CAPT Timer/Counter4 Capture Event */
	.global	atmega_tim4_compa	/* 0x0054 TIMER4 COMPA Timer/Counter4 Compare Match A */
	.global	atmega_tim4_compb	/* 0x0056 TIMER4 COMPB Timer/Counter4 Compare Match B */
	.global	atmega_tim4_compc	/* 0x0058 TIMER4 COMPC Timer/Counter4 Compare Match C */
	.global	atmega_tim4_ovf		/* 0x005A TIMER4 OVF Timer/Counter4 Overflow */
	.global	atmega_tim5_capt	/* 0x005C TIMER5 CAPT Timer/Counter5 Capture Event */
	.global	atmega_tim5_compa	/* 0x005E TIMER5 COMPA Timer/Counter5 Compare Match A */
	.global	atmega_tim5_compb	/* 0x0060 TIMER5 COMPB Timer/Counter5 Compare Match B */
	.global	atmega_tim5_compc	/* 0x0062 TIMER5 COMPC Timer/Counter5 Compare Match C */
	.global	atmega_tim5_ovf		/* 0x0064 TIMER5 OVF Timer/Counter5 Overflow */
	.global	atmega_u2rx			/* 0x0066 USART2 RX USART2 Rx Complete */
	.global	atmega_u2dre		/* 0x0068 USART2 UDRE USART2 Data Register Empty */
	.global	atmega_u2tx			/* 0x006A USART2 TX USART2 Tx Complete */
	.global	atmega_u3rx			/* 0x006C USART3 RX USART3 Rx Complete */
	.global	atmega_u3dre		/* 0x006E USART3 UDRE USART3 Data Register Empty */
	.global	atmega_u3tx			/* 0x0070 USART3 TX USART3 Tx Complete */
#elif defined(CHIP_ATMEGA328P)
	.global	atmega_int0			/* 0x0002 INT0 External Interrupt Request 0 */
	.global	atmega_int1			/* 0x0004 INT1 External Interrupt Request 1 */
	.global	atmega_pcint0		/* 0x0006 PCINT0 Pin Change Interrupt Req 0 */
	.global	atmega_pcint1		/* 0x0008 PCINT1 Pin Change Interrupt Req 1 */
	.global	atmega_pcint2		/* 0x000A PCINT2 Pin Change Interrupt Req 2 */
	.global	atmega_wdt			/* 0x000C WDT Watchdog Time-out Interrupt */
	.global	atmega_tim2_compa	/* 0x000E TIMER2 COMPA Timer/Counter2 Compare Match A */
	.global	atmega_tim2_compb	/* 0x0010 TIMER2 COMPB Timer/Counter2 Compare Match B */
	.global	atmega_tim2_ovf		/* 0x0012 TIMER2 OVF Timer/Counter2 Overflow */
	.global	atmega_tim1_capt	/* 0x0014 TIMER1 CAPT Timer/Counter1 Capture Event */
	.global	atmega_tim1_compa	/* 0x0016 TIMER1 COMPA Timer/Counter1 Compare Match A */
	.global	atmega_tim1_compb	/* 0x0018 TIMER1 COMPB Timer/Counter1 Compare Match B */
	.global	atmega_tim1_ovf		/* 0x001A TIMER1 OVF Timer/Counter1 Overflow */
	.global	atmega_tim0_compa	/* 0x001C TIMER0 COMPA Timer/Counter0 Compare Match A */
	.global	atmega_tim0_compb	/* 0x001E TIMER0 COMPB Timer/Counter0 Compare match B */
	.global	atmega_tim0_ovf		/* 0x0020 TIMER0 OVF Timer/Counter0 Overflow */
	.global	atmega_spi_stc		/* 0x0022 SPI, STC SPI Serial Transfer Complete */
	.global	atmega_u0rx			/* 0x0024 USART0 RX USART0 Rx Complete */
	.global	atmega_u0dre		/* 0x0026 USART0 UDRE USART0 Data Register Empty */
	.global	atmega_u0tx			/* 0x0028 USART0 TX USART0 Tx Complete */
	.global	atmega_adc			/* 0x002A ADC ADC Conversion Complete */
	.global	atmega_ee_rdy		/* 0x002C EE READY EEPROM Ready */
	.global	atmega_ana_comp		/* 0x002E ANALOG COMP Analog Comparator */
	.global	atmega_twi			/* 0x0030 TWI 2-wire Serial Interface */
	.global	atmega_spm_rdy		/* 0x0032 SPM READY Store Program Memory Ready */
#else
#error "Unrecognized chip"
#endif

/****************************************************************************
 * Macros
 ****************************************************************************/

	.macro	vector name
	jmp		\name
	.endm

/****************************************************************************
 * Vector Table
 ****************************************************************************/

/* The ATmega128 has 35 interrupt vectors including vector 0, the reset
 * vector.
 */

	.section .vectors, "ax", @progbits
	.func	vectortab
vectortab:
	jmp		__start				/*  0: Vector 0 is the reset vector */
#if defined(CHIP_ATMEGA128)
	vector	atmega_int0			/*  1: External interrupt request 0 */
	vector	atmega_int1			/*  2: External interrupt request 1 */
	vector	atmega_int2			/*  3: External interrupt request 2 */
	vector	atmega_int3			/*  4: External interrupt request 3 */
	vector	atmega_int4			/*  5: External interrupt request 4 */
	vector	atmega_int5			/*  6 : External interrupt request 5 */
	vector	atmega_int6			/*  7: External interrupt request 6 */
	vector	atmega_int7			/*  8: External interrupt request 7 */
	vector	atmega_t2comp		/*  9: TIMER2 COMP timer/counter2 compare match */
	vector	atmega_t2ovf		/* 10: TIMER2 OVF timer/counter2 overflow */
	vector	atmega_t1capt		/* 11: TIMER1 CAPT timer/counter1 capture event */
	vector	atmega_t1compa		/* 12: TIMER1 COMPA timer/counter1 compare match a */
	vector	atmega_t1compb		/* 13: TIMER1 COMPB timer/counter1 compare match b */
	vector	atmega_t1ovf		/* 14: TIMER1 OVF timer/counter1 overflow */
	vector	atmega_t0comp		/* 15: TIMER0 COMP timer/counter0 compare match */
	vector	atmega_t0ovf		/* 16: TIMER0 OVF timer/counter0 overflow */
	vector	atmega_spi			/* 17: STC SPI serial transfer complete */
	vector	atmega_u0rx			/* 18: USART0 RX complete */
	vector	atmega_u0dre		/* 19: USART0 data register empty */
	vector	atmega_u0tx			/* 20: USART0 TX complete */
	vector	atmega_adc			/* 21: ADC conversion complete */
	vector	atmega_ee			/* 22: EEPROM ready */
	vector	atmega_anacomp		/* 23: ANALOG COMP analog comparator */
	vector	atmega_t1compc		/* 24: TIMER1 COMPC timer/countre1 compare match c */
	vector	atmega_t3capt		/* 25: TIMER3 CAPT timer/counter3 capture event */
	vector	atmega_t3compa		/* 26: TIMER3 COMPA timer/counter3 compare match a */
	vector	atmega_t3compb		/* 27: TIMER3 COMPB timer/counter3 compare match b */
	vector	atmega_t3compc		/* 28: TIMER3 COMPC timer/counter3 compare match c */
	vector	atmega_t3ovf		/* 29: TIMER3 OVF timer/counter3 overflow */
	vector	atmega_u1rx			/* 30: USART1 RX complete */
	vector	atmega_u1dre		/* 31: USART1 data register empty */
	vector	atmega_u1tx			/* 32: USART1 TX complete */
	vector	atmega_twi			/* 33: TWI two-wire serial interface */
	vector	atmega_spmrdy		/* 34: Store program memory ready */
#elif defined(CHIP_ATMEGA1284P)
	vector	atmega_int0 		/* External interrupt request 0 */
	vector	atmega_int1 		/* External interrupt request 1 */
	vector	atmega_int2 		/* External interrupt request 2 */
	vector	atmega_pcint0 		/* Pin change interrupt request 0 */
	vector	atmega_pcint1 		/* Pin change interrupt request 1 */
	vector	atmega_pcint2 		/* Pin change interrupt request 2 */
	vector	atmega_pcint3 		/* Pin change interrupt request 3 */
	vector	atmega_wdt			/* Watchdog time-out interrupt */
	vector	atmega_t2compa		/* TIMER2 COMPA timer/counter2 compare match */
	vector	atmega_t2compb		/* TIMER2 COMPB timer/counter2 compare match */
	vector	atmega_t2ovf		/* TIMER2 OVF timer/counter2 overflow */
	vector	atmega_t1capt		/* TIMER1 CAPT timer/counter1 capture event */
	vector	atmega_t1compa		/* TIMER1 COMPA timer/counter1 compare match a */
	vector	atmega_t1compb		/* TIMER1 COMPB timer/counter1 compare match b */
	vector	atmega_t1ovf		/* TIMER1 OVF timer/counter1 overflow */
	vector	atmega_t0compa		/* TIMER0 COMPA timer/counter0 compare match */
	vector	atmega_t0compb		/* TIMER0 COMPB timer/counter0 compare match */
	vector	atmega_t0ovf		/* TIMER0 OVF timer/counter0 overflow */
	vector	atmega_spi			/* STC SPI serial transfer complete */
	vector	atmega_u0rx 		/* USART0 RX complete */
	vector	atmega_u0dre		/* USART0 data register empty */
	vector	atmega_u0tx 		/* USART0 TX complete */
	vector	atmega_anacomp		/* ANALOG COMP analog comparator */
	vector	atmega_adc			/* ADC conversion complete */
	vector	atmega_ee 			/* EEPROM ready */
	vector	atmega_twi			/* TWI two-wire serial interface */
	vector	atmega_spmrdy		/* Store program memory ready */
	vector	atmega_u1rx 		/* USART1 RX complete */
	vector	atmega_u1dre		/* USART1 data register empty */
	vector	atmega_u1tx 		/* USART1 TX complete */
	vector	atmega_t3capt 		/* TIMER3 CAPT timer/counter3 capture event */
	vector	atmega_t3compa		/* TIMER3 COMPA timer/counter3 compare match a */
	vector	atmega_t3compb		/* TIMER3 COMPB timer/counter3 compare match b */
	vector	atmega_t3ovf		/* TIMER3 OVF timer/counter3 overflow */
#elif defined(CHIP_ATMEGA2560)
	vector	atmega_int0			/* 0x0002 INT0 External Interrupt Request 0 */
	vector	atmega_int1			/* 0x0004 INT1 External Interrupt Request 1 */
	vector	atmega_int2			/* 0x0006 INT2 External Interrupt Request 2 */
	vector	atmega_int3			/* 0x0008 INT3 External Interrupt Request 3 */
	vector	atmega_int4			/* 0x000A INT4 External Interrupt Request 4 */
	vector	atmega_int5			/* 0x000C INT5  External Interrupt Request 5 */
	vector	atmega_int6			/* 0x000E INT6 External Interrupt Request 6 */
	vector	atmega_int7			/* 0x0010 INT7 External Interrupt Request 7 */
	vector	atmega_pcint0		/* 0x0012 PCINT0 Pin Change Interrupt Req 0 */
	vector	atmega_pcint1		/* 0x0014 PCINT1 Pin Change Interrupt Req 1 */
	vector	atmega_pcint2		/* 0x0016 PCINT2 Pin Change Interrupt Req 2 */
	vector	atmega_wdt			/* 0x0018 WDT Watchdog Time-out Interrupt */
	vector	atmega_tim2_compa	/* 0x001A TIMER2 COMPA Timer/Counter2 Compare Match A */
	vector	atmega_tim2_compb	/* 0x001C TIMER2 COMPB Timer/Counter2 Compare Match B */
	vector	atmega_tim2_ovf		/* 0x001E TIMER2 OVF Timer/Counter2 Overflow */
	vector	atmega_tim1_capt	/* 0x0020 TIMER1 CAPT Timer/Counter1 Capture Event */
	vector	atmega_tim1_compa	/* 0x0022 TIMER1 COMPA Timer/Counter1 Compare Match A */
	vector	atmega_tim1_compb	/* 0x0024 TIMER1 COMPB Timer/Counter1 Compare Match B */
	vector	atmega_tim1_compc	/* 0x0026 TIMER1 COMPC Timer/Counter1 Compare Match C */
	vector	atmega_tim1_ovf		/* 0x0028 TIMER1 OVF Timer/Counter1 Overflow */
	vector	atmega_tim0_compa	/* 0x002A TIMER0 COMPA Timer/Counter0 Compare Match A */
	vector	atmega_tim0_compb	/* 0x002C TIMER0 COMPB Timer/Counter0 Compare match B */
	vector	atmega_tim0_ovf		/* 0x002E TIMER0 OVF Timer/Counter0 Overflow */
	vector	atmega_spi_stc		/* 0x0030 SPI, STC SPI Serial Transfer Complete */
	vector	atmega_u0rx			/* 0x0032 USART0 RX USART0 Rx Complete */
	vector	atmega_u0dre		/* 0x0034 USART0 UDRE USART0 Data Register Empty */
	vector	atmega_u0tx			/* 0x0036 USART0 TX USART0 Tx Complete */
	vector	atmega_ana_comp		/* 0x0038 ANALOG COMP Analog Comparator */
	vector	atmega_adc			/* 0x003A ADC ADC Conversion Complete */
	vector	atmega_ee_rdy		/* 0x003C EE READY EEPROM Ready */
	vector	atmega_tim3_capt	/* 0x003E TIMER3 CAPT Timer/Counter3 Capture Event */
	vector	atmega_tim3_compa	/* 0x0040 TIMER3 COMPA Timer/Counter3 Compare Match A */
	vector	atmega_tim3_compb	/* 0x0042 TIMER3 COMPB Timer/Counter3 Compare Match B */
	vector	atmega_tim3_compc	/* 0x0044 TIMER3 COMPC Timer/Counter3 Compare Match C */
	vector	atmega_tim3_ovf		/* 0x0046 TIMER3 OVF Timer/Counter3 Overflow */
	vector	atmega_u1rx			/* 0x0048 USART1 RX USART1 Rx Complete */
	vector	atmega_u1dre		/* 0x004A USART1 UDRE USART1 Data Register Empty */
	vector	atmega_u1tx			/* 0x004C USART1 TX USART1 Tx Complete */
	vector	atmega_twi			/* 0x004E TWI 2-wire Serial Interface */
	vector	atmega_spm_rdy		/* 0x0050 SPM READY Store Program Memory Ready */
	vector	atmega_tim4_capt	/* 0x0052 TIMER4 CAPT Timer/Counter4 Capture Event */
	vector	atmega_tim4_compa	/* 0x0054 TIMER4 COMPA Timer/Counter4 Compare Match A */
	vector	atmega_tim4_compb	/* 0x0056 TIMER4 COMPB Timer/Counter4 Compare Match B */
	vector	atmega_tim4_compc	/* 0x0058 TIMER4 COMPC Timer/Counter4 Compare Match C */
	vector	atmega_tim4_ovf		/* 0x005A TIMER4 OVF Timer/Counter4 Overflow */
	vector	atmega_tim5_capt	/* 0x005C TIMER5 CAPT Timer/Counter5 Capture Event */
	vector	atmega_tim5_compa	/* 0x005E TIMER5 COMPA Timer/Counter5 Compare Match A */
	vector	atmega_tim5_compb	/* 0x0060 TIMER5 COMPB Timer/Counter5 Compare Match B */
	vector	atmega_tim5_compc	/* 0x0062 TIMER5 COMPC Timer/Counter5 Compare Match C */
	vector	atmega_tim5_ovf		/* 0x0064 TIMER5 OVF Timer/Counter5 Overflow */
	vector	atmega_u2rx			/* 0x0066 USART2 RX USART2 Rx Complete */
	vector	atmega_u2dre		/* 0x0068 USART2 UDRE USART2 Data Register Empty */
	vector	atmega_u2tx			/* 0x006A USART2 TX USART2 Tx Complete */
	vector	atmega_u3rx			/* 0x006C USART3 RX USART3 Rx Complete */
	vector	atmega_u3dre		/* 0x006E USART3 UDRE USART3 Data Register Empty */
	vector	atmega_u3tx			/* 0x0070 USART3 TX USART3 Tx Complete */
#elif defined(CHIP_ATMEGA328P)
	vector	atmega_int0			/* 0x0002 INT0 External Interrupt Request 0 */
	vector	atmega_int1			/* 0x0004 INT1 External Interrupt Request 1 */
	vector	atmega_pcint0		/* 0x0006 PCINT0 Pin Change Interrupt Req 0 */
	vector	atmega_pcint1		/* 0x0008 PCINT1 Pin Change Interrupt Req 1 */
	vector	atmega_pcint2		/* 0x000A PCINT2 Pin Change Interrupt Req 2 */
	vector	atmega_wdt			/* 0x000C WDT Watchdog Time-out Interrupt */
	vector	atmega_tim2_compa	/* 0x000E TIMER2 COMPA Timer/Counter2 Compare Match A */
	vector	atmega_tim2_compb	/* 0x0010 TIMER2 COMPB Timer/Counter2 Compare Match B */
	vector	atmega_tim2_ovf		/* 0x0012 TIMER2 OVF Timer/Counter2 Overflow */
	vector	atmega_tim1_capt	/* 0x0014 TIMER1 CAPT Timer/Counter1 Capture Event */
	vector	atmega_tim1_compa	/* 0x0016 TIMER1 COMPA Timer/Counter1 Compare Match A */
	vector	atmega_tim1_compb	/* 0x0018 TIMER1 COMPB Timer/Counter1 Compare Match B */
	vector	atmega_tim1_ovf		/* 0x001A TIMER1 OVF Timer/Counter1 Overflow */
	vector	atmega_tim0_compa	/* 0x001C TIMER0 COMPA Timer/Counter0 Compare Match A */
	vector	atmega_tim0_compb	/* 0x001E TIMER0 COMPB Timer/Counter0 Compare match B */
	vector	atmega_tim0_ovf		/* 0x0020 TIMER0 OVF Timer/Counter0 Overflow */
	vector	atmega_spi_stc		/* 0x0022 SPI, STC SPI Serial Transfer Complete */
	vector	atmega_u0rx			/* 0x0024 USART0 RX USART0 Rx Complete */
	vector	atmega_u0dre		/* 0x0026 USART0 UDRE USART0 Data Register Empty */
	vector	atmega_u0tx			/* 0x0028 USART0 TX USART0 Tx Complete */
	vector	atmega_adc			/* 0x002A ADC ADC Conversion Complete */
	vector	atmega_ee_rdy		/* 0x002C EE READY EEPROM Ready */
	vector	atmega_ana_comp		/* 0x002E ANALOG COMP Analog Comparator */
	vector	atmega_twi			/* 0x0030 TWI 2-wire Serial Interface */
	vector	atmega_spm_rdy		/* 0x0032 SPM READY Store Program Memory Ready */
#else
#	error "Unrecognized chip"
#endif
	.endfunc