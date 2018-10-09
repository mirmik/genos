#ifndef GENOS_DRIVERS_AVR_UART
#define GENOS_DRIVERS_AVR_UART

#include <drivers/serial/uart.h>
#include <periph/regs/usart.h>

struct avr_uart {
	struct uart u;

};

int avr_uart_init(struct avr_uart * dev, )

#endif