#ifndef AVR2560_USART_H
#define AVR2560_USART_H

#include <hal/reg.h>
#include <kernel/irq.h>

struct usart_regs {
	reg_t ucsr_a;
	reg_t ucsr_b;
	reg_t ucsr_c;
	reg_t reserve0;
	reg_t ubrr_l;
	reg_t ubrr_h;
	reg_t udr;
} __attribute__((packed));

#endif 