#ifndef GENOS_USART_DRIVER_H
#define GENOS_USART_DRIVER_H

#include <sys/cdefs.h>

#include <asm/usart.h>
#include <drivers/serial/uart_drvbh.h>
#include <datastruct/ring_head.h>

struct usart_drvbh {
	const struct usart_data udata;
	
	char* tx_buffer;
	struct ring_head tx_ring;

	void(*rx_headler)(struct stream_servicechar c);
	void(*tx_empty_headler)();
};

typedef struct usart_drvbh usart_drvbh_t;

__BEGIN_DECLS

static int usart_drvbh_init(usart_drvbh_t * udrv, const struct usart_data * udata, 
	char* txbuffer, size_t bufsz) 
{
	udrv -> udata = udata;
	udrv -> tx_buffer = txbuffer;
	ring_init(&udrv->tx_ring, bufsz);
}

static int usart_drvbh_write(usart_drvbh_t * udrv, const char* data, size_t size) {

}

__END_DECLS

struct strmdevice;

struct strmdevice_operations {
	int(*io_write)(struct strmdevice*, const char* data, size_t sz);
	int(*io_read)(struct strmdevice*, const char* data, size_t sz);
};

struct strmdevice {

};

#endif