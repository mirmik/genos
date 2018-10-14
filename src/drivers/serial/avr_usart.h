#ifndef GENOS_DRIVERS_AVR_UART
#define GENOS_DRIVERS_AVR_UART

#include <drivers/serial/uart.h>
#include <periph/regs/usart.h>

#include <hal/irqtable.h>

struct avr_usart : public uart {
	struct usart_regs * regs;
	uint8_t base_irq;

	int enable(bool en) override;
	int txirq(bool en) override;
	int getc() override;
	int putc(int symbol) override;
	int cantx() override;
	int hasrx() override;
	int setup(const struct uart_params *params) override;

	int init(struct usart_regs * regs, int base_irq);
};

__BEGIN_DECLS

__END_DECLS

#endif