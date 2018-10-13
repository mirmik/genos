#ifndef GENOS_DATASTRUCT_UARTRING_H
#define GENOS_DATASTRUCT_UARTRING_H

#include <sys/cdefs.h>

#include <drivers/cdev/cdev.h>
#include <mvfs/file.h>

#include <gxx/datastruct/ring.h>

#include <drivers/serial/uart.h>

#define UARTRING_BUFFER_SIZE_RX 8
#define UARTRING_BUFFER_SIZE_TX 16

struct uartring_device : public char_device {
	struct uart * uart;
	
	char rxbuffer[UARTRING_BUFFER_SIZE_RX];
	char txbuffer[UARTRING_BUFFER_SIZE_TX];
	struct ring_head rxring;
	struct ring_head txring;

	struct dlist_head txwlst;

	int init(struct uart * u, const char* name);

	int write(const char* data, unsigned int size);

	//file operations
	int write(struct file * f, const char* data, unsigned int size);	
	int read(struct file * f, char* data, unsigned int size);	
	int open(struct node * i, struct file * f);	
	int release (struct node * i, struct file * f);
};

__BEGIN_DECLS

__END_DECLS

#endif