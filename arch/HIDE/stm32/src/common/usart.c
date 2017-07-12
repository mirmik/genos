#include "drivers/haldrivers/usart.h"
#include <genos/irq.h>
#include <util/stub.h>
#include <util/retype.h>

inline int usart_putc(struct usart_regs * usart, char c) {
	usart->DR = c;
	return 1;
}

inline int usart_getc(struct usart_regs * usart) {
	return usart->DR;
}

inline int usart_room(struct usart_regs * usart) {
	return usart->SR & (1 << 7); //TXNE byte
}

inline int usart_avail(struct usart_regs * usart) {
	return usart->SR & (1 << 5); //RXNE byte
}

inline int usart_rx_irq_enable(struct usart_regs * usart, uint8_t stat) {
	debug_print("usart_rx_irq_enable");
}

inline int usart_tx_irq_enable(struct usart_regs * usart, uint8_t stat) {
	debug_print("usart_tx_irq_enable");
}

inline int usart_tx_complete_irq_enable(struct usart_regs * usart, uint8_t stat) {
	debug_print("usart_tx_complete_irq_enable");
}

#include <drivers/serial/stm32_uart.h>
#include <mem/alloc.h>

int stm32_uart_putc(struct uart* u, char c) {
	struct stm32_uart* su = (struct stm32_uart*) u;
	//while(!usart_room(su->regs));
	return usart_putc(su->regs, c);
}

int stm32_uart_getc(struct uart* u) {
	struct stm32_uart* su = (struct stm32_uart*) u;
	//while(!usart_avail(su->regs));
	return usart_getc(su->regs);	
}

int stm32_uart_room(struct uart* u) {
	struct stm32_uart* su = (struct stm32_uart*) u;
	return usart_room(su->regs);
}

int stm32_uart_avail(struct uart* u) {
	struct stm32_uart* su = (struct stm32_uart*) u;
	return usart_avail(su->regs);
}

int stm32_uart_setup(struct uart* u, const struct uart_params* params) {
	debug_print("stm32_uart_setup::TODO\r\n");
}

int stm32_uart_txbegin(struct uart* u) {
	debug_print("stm32_uart_txbegin::TODO\r\n");
}

int stm32_uart_txend(struct uart* u) {
	debug_print("stm32_uart_txend::TODO\r\n");
}

//void stm32_uart_irq_rx_handler(void* uart) {
//	RETYPE(struct stm32_uart*, su, uart);
//	su->parent.drvhs->rx(su->parent.drvpriv, stm32_usart_getc(su));
//}

//void stm32_uart_irq_tx_handler(void* uart) {
//	RETYPE(struct stm32_uart*, su, uart);
//	su->parent.drvhs->tx(su->parent.drvpriv);
//}

//void stm32_uart_irq_tx_complete_handler(void* uart) {
//	RETYPE(struct stm32_uart*, su, uart);
//	su->parent.drvhs->tx_complete(su->parent.drvpriv);
//}

void stm32_uart_irq_handler() {
	debug_print("UART_IRQ_HANDLER");
}

struct uart_ops stm32_uart_ops = {
	.putc = stm32_uart_putc,
	.getc = stm32_uart_getc,

	.room = stm32_uart_room,
	.avail = stm32_uart_avail,

	.setup = stm32_uart_setup,

	.txbegin = stm32_uart_txbegin, 
	.txend = stm32_uart_txend,
};

struct uart_driver_handlers uart_driver_handlers_stub = {
	.rx = do_nothing,
	.tx = do_nothing,
	.tx_complete = do_nothing,
};

int stm32_uart_irqs_init(struct stm32_uart* su);

int stm32_uart_init(struct stm32_uart* su, struct usart_regs* regs, uint8_t irqno) {
	su->regs = regs;
	su->parent.uops = &stm32_uart_ops;
	
	su->parent.drvhs = &uart_driver_handlers_stub;
	su->parent.drvpriv = NULL;

	su->irqno = irqno;
	stm32_uart_irqs_init(su);
}

int stm32_uart_irqs_init(struct stm32_uart* su) {
	setIRQHandler(su->irqno, stm32_uart_irq_handler, su);	
	usart_rx_irq_enable(su->regs, 1);
}

