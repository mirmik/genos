#ifndef UART_DEVICE_H_
#define UART_DEVICE_H_

#include <drivers/serial/settings.h>
#include <stdbool.h>

/**
 *int (*uart_getc)(struct uart *dev);
 *
 *
 *int (*uart_putc)(struct uart *dev, int symbol, bool islast);
 *
 *
 *int (*uart_hasrx)(struct uart *dev);
 *	This function requested hardware about availabe byte.
 *
 *int (*uart_cantx)(struct uart *dev);
 *	This function requested hardware can we send another one byte.
 *
 *int (*uart_setup)(struct uart *dev, const struct uart_params *params);
 *	Enable IO, and activate RX irq.
 *
 *int (*uart_enable_tx)(struct uart *dev, bool en);
 *	Control transmit line for prevent infinity irq.	
 */

struct uart;

#define UART_IRQCODE_RX 0
#define UART_IRQCODE_TX 1

struct uart_operations {
	int (*enable)(struct uart *dev, bool en);
	int (*getc)(struct uart *dev);
	int (*putc)(struct uart *dev, int symbol, bool islast);
	int (*hasrx)(struct uart *dev);
	int (*setup)(struct uart *dev, const struct uart_params *params);
};

struct uart {
	void(*handler)(int);
	const struct uart_operations * u_op;
};

__BEGIN_DECLS


static inline int uart_enable(struct uart * dev, bool en) {
	return dev->u_op->enable(dev, en);
}

static inline int uart_getc(struct uart *dev) {
	return dev->u_op->getc(dev);
}

static inline int uart_putc(struct uart *dev, int symbol, bool last) {
	return dev->u_op->putc(dev, symbol, last);
}

static inline int uart_hasrx(struct uart *dev) {
	return dev->u_op->hasrx(dev);
}

static inline int uart_setup(struct uart *dev, const struct uart_params *params) {
	return dev->u_op->setup(dev, params);	
}

__END_DECLS

#endif
