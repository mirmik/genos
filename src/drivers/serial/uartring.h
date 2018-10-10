#ifndef GENOS_DATASTRUCT_UARTRING_H
#define GENOS_DATASTRUCT_UARTRING_H

#define UARTRING_BUFFER_SIZE_RX 8
#define UARTRING_BUFFER_SIZE_TX 16

struct uartring_driver {
	struct uart * uart;
	
	char rxbuffer[UARTRING_BUFFER_SIZE_RX]
	char txbuffer[UARTRING_BUFFER_SIZE_TX];
	struct ring_head rxring;
	struct ring_head txring;

	struct tty * tty; 
}

__BEGIN_DECLS

/*static inline int uartring_open(struct uartring_device* dev) {
	if (dev->refs++ == 0) {}
	return 0;
}

int uartring_release(struct uartring_device* dev) {
	if (--dev->refs == 0) {}
}*/

int uartring_write(struct uartring_device*, const char* data, unsigned int size) {

}

int uartring_read(struct uartring_device*, char* data, unsigned int size) {

}

void uartring_irq_handler(void* priv, int code) {
	struct uartring_driver
}

static inline int uartring_init(struct uartring_device* dev) {
	ring_init(&dev->rxring, UARTRING_BUFFER_SIZE_RX);
	ring_init(&dev->txring, UARTRING_BUFFER_SIZE_TX);

	uart_enable(uart, false);

	uart->handler = uartring_irq_handler;
	uart->handarg = (void*) dev;
	uart_txirq(uart, false);
	
	uart_enable(uart, true);
}

//static int uartring_  

__END_DECLS

#endif