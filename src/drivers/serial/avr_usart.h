#ifndef GENOS_DRIVERS_AVR_UART
#define GENOS_DRIVERS_AVR_UART

#include <drivers/serial/uart.h>
#include <periph/regs/usart.h>

#include <hal/irqtable.h>

struct avr_usart {
	struct uart u;
	struct usart_regs * regs;
	uint8_t base_irq;
};

__BEGIN_DECLS

int avr_usart_init(struct avr_usart * dev, struct usart_regs * regs, int base_irq);

__END_DECLS

#endif