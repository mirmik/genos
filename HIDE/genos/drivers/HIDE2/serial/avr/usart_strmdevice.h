#ifndef AVR_USART_STRMDEVICE_H
#define AVR_USART_STRMDEVICE_H

#include <drivers/serial/strmdevice.h>

struct avr_usart_strmdev {
	struct strmdevice strmdev;

	const struct usart_data udata;
	
	char* tx_buffer;
	struct ring_head tx_ring;

	void(*rx_headler)(struct stream_servicechar c);
	void(*tx_empty_headler)();
};

#endif