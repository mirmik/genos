#ifndef UART_DEVICE_H_
#define UART_DEVICE_H_

#include <drivers/serial/settings.h>
#include <stdbool.h>

struct uart;

#define UART_IRQCODE_RX 0
#define UART_IRQCODE_TX 1

struct uart_operations {
	int (*enable)(struct uart *dev, bool en);
	int (*txirq)(struct uart *dev, bool en);
	int (*getc)(struct uart *dev);
	int (*putc)(struct uart *dev, int symbol);
	int (*hasrx)(struct uart *dev);
	int (*setup)(struct uart *dev, const struct uart_params *params);
};

///uart low-half driver
struct uart {
	void(*handler)(int);
	const struct uart_operations * u_op;
};

__BEGIN_DECLS


static inline int uart_enable(struct uart * dev, bool en) {
	return dev->u_op->enable(dev, en);
}

static inline int uart_txirq(struct uart * dev, bool en) {
	return dev->u_op->txirq(dev, en);
}

static inline int uart_getc(struct uart *dev) {
	return dev->u_op->getc(dev);
}

static inline int uart_putc(struct uart *dev, int symbol) {
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
