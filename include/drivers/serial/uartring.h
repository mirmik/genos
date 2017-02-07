#ifndef AVR_UART_DEV
#define AVR_UART_DEV

#include <compiler.h>
#include <drivers/serial/uartdev.h>

struct uartring_s {
	struct uartdev_s udev;

	void* rxbuffer;

	void* txbuffer;
};
typedef struct uartring_s uartring_t;

__BEGIN_DECLS

extern const struct chardev_operations uartring_chardev_ops;

static inline void uartring_init(struct uartring_s * dev, 
	const struct uart_operations * uops,
	const struct uart_irq_handlers * uirqops, 
	void* upriv) 
{
	uartdev_init(&dev->udev, &uartring_chardev_ops, uops, uirqops, upriv);
}

__END_DECLS

#endif