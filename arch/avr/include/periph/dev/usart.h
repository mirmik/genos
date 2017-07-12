#ifndef AVR_PERIPH_USART_DEV_H
#define AVR_PERIPH_USART_DEV_H

#include <periph/regs/usart.h>
#include <drivers/serial/uart.h>

#include <datastruct/ring_head.h>
#include <genos/compiler.h>

#define USART_TXBUFSIZE 128

struct usart_irqs {
	uint8_t tx;
	uint8_t rx;
	uint8_t udre;
};

struct usart_device {
	struct usart_regs* 	regs;
	struct usart_irqs 	irqs;

	char txbuffer[USART_TXBUFSIZE];
	struct ring_head txring;
};

__BEGIN_DECLS

void usart_driver_register(struct uart_driver* uart, struct usart_device* dev);

__END_DECLS

#endif