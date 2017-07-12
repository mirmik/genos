#ifndef AVR_UART_DRIVER_H
#define AVR_UART_DRIVER_H

#include <periph/usart.h>
#include <drivers/serial/uartring.h>

extern const struct uart_operations avr_usart_ops;

__BEGIN_DECLS

static inline void avr_uartring_init(struct uartring_s * dev, void* txbuf, size_t txsz, void* rxbuf, size_t rxsz, const struct usart_data* udata) {
	uartring_init(dev, txbuf, txsz, rxbuf, rxsz, &avr_usart_ops, (void*) udata);
}

__END_DECLS

#endif