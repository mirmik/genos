#ifndef AVR2560_TIMER_REGS
#define AVR2560_TIMER_REGS

#include <inttypes.h>

struct timsk_array {
	volatile uint8_t timsk[5];
}; 

struct timer_8bit_regs {
	volatile uint8_t tccr_a;
	volatile uint8_t tccr_b;
	volatile uint8_t tcnt;
	volatile uint8_t ocr_a;
	volatile uint8_t ocr_b;
} __attribute__((packed));


struct timer_16bit_regs {
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

#define TIMER0 ((struct timer_8bit_regs *)(TIMER_BASE_0))
#define TIMER2 ((struct timer_8bit_regs *)(TIMER_BASE_2))

#define TIMER1 ((struct timer_16bit_regs *)(TIMER_BASE_1))
#define TIMER3 ((struct timer_16bit_regs *)(TIMER_BASE_3))
#define TIMER4 ((struct timer_16bit_regs *)(TIMER_BASE_4))
#define TIMER5 ((struct timer_16bit_regs *)(TIMER_BASE_5))

#define TIMSK (((struct timsk_array *)(0x6E)) -> timsk)

#endif