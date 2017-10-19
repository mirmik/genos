#ifndef AVR_UART_DEV
#define AVR_UART_DEV

#include <sys/cdefs.h>
#include <drivers/serial/uartdev.h>
#include <datastruct/ring_head.h>

struct uartring_s {
	struct uartdev_s udev;

	void* rxbuf;
	struct ring_head rxring;

	void* txbuf;
	struct ring_head txring;
};
typedef struct uartring_s uartring_t;

__BEGIN_DECLS

extern const struct chardev_operations uartring_chardev_ops;
extern const struct uart_irq_handlers uartring_uartdev_irqops;

static inline void uartring_init(struct uartring_s * dev, void* txbuf, size_t txsz, void* rxbuf, size_t rxsz, const struct uart_operations * uops, void* upriv) {
	ring_init(&dev->rxring, rxsz);
	ring_init(&dev->txring, txsz);
	dev->rxbuf = rxbuf;
	dev->txbuf = txbuf;
	uartdev_init(&dev->udev, &uartring_chardev_ops, uops, &uartring_uartdev_irqops, upriv);
}

__END_DECLS

#endif