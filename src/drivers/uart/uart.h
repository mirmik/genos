#ifndef UART_DEVICE_H_
#define UART_DEVICE_H_

#include <stdint.h>

enum uart_parity_e {
	UART_PARITY_NONE = 0,
	UART_PARITY_EVEN = 1,
	UART_PARITY_ODD = 2,
};

struct uart_params {
	uint32_t baud_rate;
	enum uart_parity_e parity;
	int n_stop;
	int n_bits;
};

struct uart_head {
	short irq_num;
	uint32_t base_addr;
};

/**
 *int (*uart_getc)(struct uart *dev);
 *
 *
 *int (*uart_putc)(struct uart *dev, int symbol);
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

struct uart_operations {
	int (*uart_getc)(struct uart *dev);
	int (*uart_putc)(struct uart *dev, int symbol);
	int (*uart_hasrx)(struct uart *dev);
	int (*uart_setup)(struct uart *dev, const struct uart_params *params);
	int (*uart_enable_tx)(struct uart *dev, bool en);
};

#endif
