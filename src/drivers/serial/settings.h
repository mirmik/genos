#ifndef GENOS_SERIAL_SETTINGS_H
#define GENOS_SERIAL_SETTINGS_H

#include <stdint.h>

enum uart_parity_e {
	UART_PARITY_NONE = 0,
	UART_PARITY_EVEN = 1,
	UART_PARITY_ODD = 2,
};

struct uart_params {
	uint32_t 			baud;
	enum uart_parity_e 	parity;
	int 				nstop;
	int 				nbits;
};

#endif