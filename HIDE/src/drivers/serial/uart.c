#include <drivers/serial/uart.h>

const struct uart_params uart_default_params = {
	9600,
	UART_PARITY_NONE,
	1,
	8,
};