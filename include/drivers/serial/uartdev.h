#ifndef USART_DRIVER_H
#define USART_DRIVER_H

#include <kernel/fs/chardev.h>
#include <stdbool.h>

struct uartdev_s;

enum uart_parity {
	UartParityNone,
	UartParityEven,
	UartParityOdd,
};	

struct uart_params {
	uint32_t baudRate;
	enum uart_parity parity;
	uint8_t stopBits;
	uint8_t dataBits;
};

typedef void (*irq_handler_usart_rx_t)(struct uartdev_s*, char); 
typedef void (*irq_handler_usart_tx_t)(struct uartdev_s*);
typedef void (*irq_handler_usart_tc_t)(struct uartdev_s*);

struct uart_irq_handlers {
	irq_handler_usart_rx_t rx_handler;
	irq_handler_usart_tx_t tx_handler;
	irq_handler_usart_tc_t tc_handler;
};

typedef int(*uart_putc_t)(struct uartdev_s*, char);
typedef int(*uart_getc_t)(struct uartdev_s*);
typedef int(*uart_avail_t)(struct uartdev_s*);
typedef int(*uart_room_t)(struct uartdev_s*);

typedef int(*uart_setup_t)(struct uartdev_s*, struct uart_params* params);
typedef int(*uart_startup_t)(struct uartdev_s*);

typedef int(*uart_irq_enable_rx_t)(struct uartdev_s*, bool en);
typedef int(*uart_irq_enable_tx_t)(struct uartdev_s*, bool en);
typedef int(*uart_irq_enable_tc_t)(struct uartdev_s*, bool en);


struct uart_operations {
	uart_putc_t putc;
	uart_getc_t getc;
	uart_avail_t avail;
	uart_room_t room;

	uart_startup_t startup;
	uart_setup_t setup;

	uart_irq_enable_rx_t irq_enable_rx;
	uart_irq_enable_rx_t irq_enable_tx;
	uart_irq_enable_rx_t irq_enable_tc;
};

struct uartdev_s {
	struct chardev_s cdev;
	const struct uart_operations * uops;
	const struct uart_irq_handlers * uirqops;
	const void* upriv;
};
typedef struct uartdev_s uartdev_t;

__BEGIN_DECLS

static inline void uartdev_init(struct uartdev_s* udev, 
	const struct chardev_operations * cops, 
	const struct uart_operations * uops,
	const struct uart_irq_handlers * uirqops,
	const void* upriv
) {
	udev->uops = uops;
	udev->uirqops = uirqops;
	udev->upriv = upriv;
	chardev_init(&udev->cdev, cops);
}

__END_DECLS

#endif