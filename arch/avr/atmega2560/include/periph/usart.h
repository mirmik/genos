#ifndef AVR2560_USART_CLASS_H
#define AVR2560_USART_CLASS_H

#include <periph/regs/usart.h>

#define USART_BASE_0 0xC0
#define USART_BASE_1 0xC8
#define USART_BASE_2 0xD0

#define USART0 ((struct usart_regs *)(USART_BASE_0))
#define USART1 ((struct usart_regs *)(USART_BASE_1))
#define USART2 ((struct usart_regs *)(USART_BASE_2))

struct usart_data {
	struct usart_regs* regs;
	int8_t irqbase;
};

static const struct usart_data usart0_data = { USART0, ATMEGA_IRQ_U0RX };
static const struct usart_data usart1_data = { USART1, ATMEGA_IRQ_U1RX };
static const struct usart_data usart2_data = { USART2, ATMEGA_IRQ_U2RX };

#endif
