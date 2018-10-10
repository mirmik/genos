#ifndef GENOS_DRIVERS_AVR_UART
#define GENOS_DRIVERS_AVR_UART

#include <drivers/serial/uart.h>
#include <periph/regs/usart.h>

#include <hal/irqtable.h>

struct avr_uart {
	struct uart u;
	struct uart_regs * regs;
	uint8_t base_irq;
};

__BEGIN_DECLS

int avr_uart_init(struct avr_uart * dev, struct uart_regs * regs, int base_irq);

__END_DECLS

#endif