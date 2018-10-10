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

int avr_uart_enable(struct uart * dev, bool en) {
	struct avr_uart* uart = mcast_out(dev, avr_uart, u);
	usart_regs_enable_rx(uart->regs, en);
	usart_regs_enable_rx(uart->regs, en);
	return 0;
}

int avr_uart_txirq(struct uart * dev, bool en) {
	struct avr_uart* uart = mcast_out(dev, avr_uart, u);
	return usart_regs_txirq(uart->regs, en);
}

int avr_uart_getc(struct uart *dev) {
	struct avr_uart* uart = mcast_out(dev, avr_uart, u);
	return usart_regs_recvbyte(uart->regs);
}

int avr_uart_putc(struct uart *dev, int symbol) {
	struct avr_uart* uart = mcast_out(dev, avr_uart, u);
	return usart_regs_sendbyte(uart->regs, symbol);
}

int avr_uart_hasrx(struct uart *dev) {
	struct avr_uart* uart = mcast_out(dev, avr_uart, u);
	return usart_regs_hasrx(uart->regs);	
}

int avr_uart_setup(struct uart *dev, const struct uart_params *params) {
	struct avr_uart* uart = mcast_out(dev, avr_uart, u);
	return usart_regs_setup(uart->regs, params);		
}

const uart_operations avr_uart_operations = {
	.enable = avr_uart_enable,
	.txirq = txirq,
	.getc = avr_uart_getc,
	.putc = avr_uart_putc,
	.hasrx = avr_uart_hasrx,
	.setup = avr_uart_setup
}

int avr_uart_init(struct avr_uart * dev, struct uart_regs * regs, int base_irq) {
	dev->regs = regs;
	dev->base_irq = base_irq;
	dev->u->u_op = &avr_uart_operations;
};

#endif