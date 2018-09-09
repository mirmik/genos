#ifndef GENOS_DATASTRUCT_UARTRING_H
#define GENOS_DATASTRUCT_UARTRING_H

#define UARTRING_BUFFER_SIZE_RX 8
#define UARTRING_BUFFER_SIZE_TX 16

struct uartring_device {
	struct uart_head uart;
	const struct uart_operations* u_ops; 

	//data
	char rxbuffer[UARTRING_BUFFER_SIZE_RX]
	char txbuffer[UARTRING_BUFFER_SIZE_TX];
	struct ring_head rxring;
	struct ring_head txring;

	//manage
	struct uart_params u_params;
	uint8_t refs;
}

__BEGIN_DECLS

static inline int uartring_init(struct uartring_device* dev) {
	dev->refs = 0;

	ring_init(&dev->rxring, UARTRING_BUFFER_SIZE_RX);
	ring_init(&dev->txring, UARTRING_BUFFER_SIZE_TX);

}

static inline int uartring_open(struct uartring_device* dev) {
	if (dev->refs++ == 0) {}
	return 0;
}

int uartring_release(struct uartring_device* dev) {
	if (--dev->refs == 0) {}
}

int uartring_write(struct uartring_device*, const char* data, unsigned int size) {

}

int uartring_read(struct uartring_device*, char* data, unsigned int size) {

}

static int uartring_  

__END_DECLS

#endif