#ifndef AVR2560_USART_H
#define AVR2560_USART_H

#include <hal/reg.h>
#include <kernel/irq.h>

struct usart_regs {
	volatile uint8_t ucsr_a;
	volatile uint8_t ucsr_b;
	volatile uint8_t ucsr_c;
	volatile uint8_t reserve0;
	volatile uint8_t ubrr_l;
	volatile uint8_t ubrr_h;
	volatile uint8_t udr;
} __attribute__((packed));

#endif 