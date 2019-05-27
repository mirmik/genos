#ifndef AVR2560_TIMER_REGS
#define AVR2560_TIMER_REGS

#include <inttypes.h>

struct timsk_array {
#if defined (CHIP_ATMEGA2560)
	volatile uint8_t timsk[5];
#elif defined (CHIP_ATMEGA328P)
	volatile uint8_t timsk[2];
#else 
#	error "unrecognized chip"
#endif
}; 

struct timer8_regs {
	volatile uint8_t tccr_a;
	volatile uint8_t tccr_b;
	volatile uint8_t tcnt;
	volatile uint8_t ocr_a;
	volatile uint8_t ocr_b;
} __attribute__((packed));


struct timer16_regs {
	 volatile uint8_t tccr_a;
	 volatile uint8_t tccr_b;	
	 volatile uint8_t tccr_c;
	 volatile uint8_t reserved;
	 volatile uint16_t tcnt; 
	 volatile uint16_t icr;
	 volatile uint16_t ocr_a;
	 volatile uint16_t ocr_b;
	 volatile uint16_t ocr_c;
} __attribute__((packed));

#define TIMER_DIVIDER_1 1
#define TIMER_DIVIDER_8 2
#define TIMER_DIVIDER_64 3
#define TIMER_DIVIDER_256 4
#define TIMER_DIVIDER_1024 5

#define TIMERS_8BIT_MAP (1<<2 | 1<<0)
#define TIMERS_16BIT_MAP (1<<1 | 1<<3 | 1<<4 | 1<<5)
#define TIMERS_TOTAL 6

#define TIMER_BASE_5 0x120
#define TIMER_BASE_4 0xA0
#define TIMER_BASE_3 0x90
#define TIMER_BASE_2 0xB0
#define TIMER_BASE_1 0x80
#define TIMER_BASE_0 0x44

#if defined (CHIP_ATMEGA2560)
#define TIMER0 ((struct timer8_regs *)(TIMER_BASE_0))
#define TIMER2 ((struct timer8_regs *)(TIMER_BASE_2))
#define TIMER1 ((struct timer16_regs *)(TIMER_BASE_1))
#define TIMER3 ((struct timer16_regs *)(TIMER_BASE_3))
#define TIMER4 ((struct timer16_regs *)(TIMER_BASE_4))
#define TIMER5 ((struct timer16_regs *)(TIMER_BASE_5))
#elif defined (CHIP_ATMEGA328P)
#define TIMER0 ((struct timer8_regs *)(TIMER_BASE_0))
#define TIMER2 ((struct timer8_regs *)(TIMER_BASE_2))
#define TIMER1 ((struct timer16_regs *)(TIMER_BASE_1))
#else 
#	error "unrecognized chip"
#endif

#define TIMSK (((struct timsk_array *)(0x6E)) -> timsk)

#endif