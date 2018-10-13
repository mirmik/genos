#ifndef GENOS_DATASTRUCT_UARTRING_H
#define GENOS_DATASTRUCT_UARTRING_H

#include <sys/cdefs.h>
#include <drivers/cdev/cdev.h>
#include <gxx/datastruct/ring.h>


#define UARTRING_BUFFER_SIZE_RX 8
#define UARTRING_BUFFER_SIZE_TX 16

struct uartring {
	struct cdev cdev;

	struct uart * uart;
	
	char rxbuffer[UARTRING_BUFFER_SIZE_RX];
	char txbuffer[UARTRING_BUFFER_SIZE_TX];
	struct ring_head rxring;
	struct ring_head txring;

//	struct tty * tty; 
};

__BEGIN_DECLS

int uartring_write(struct uartring_device*, const char* data, unsigned int size);
int uartring_read(struct uartring_device*, char* data, unsigned int size);
void uartring_irq_handler(void*);

static inline int uartring_init(struct uartring_device* dev) {
	ring_init(&dev->rxring, UARTRING_BUFFER_SIZE_RX);
	ring_init(&dev->txring, UARTRING_BUFFER_SIZE_TX);

	uart_enable(uart, false);

	uart->handler = uartring_irq_handler;
	uart->handarg = (void*) dev;
	uart_txirq(uart, false);
	
	uart_enable(uart, true);
}

__END_DECLS

#endif