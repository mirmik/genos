#ifndef GENOS_AVR_USART_H
#define GENOS_AVR_USART_H

#include <avr/io.h>
#include "util/bits.h"

#include <periph/usart.h>

#include <drivers/serial/uartdev.h>
#include <drivers/serial/avr/usart.h>

void avr_usart_interrupt_handler_rx(uartdev_t * dev);
void avr_usart_interrupt_handler_tx(uartdev_t * dev);
void avr_usart_interrupt_handler_tc(uartdev_t * dev);

int avr_usart_putc(uartdev_t * dev, char c) {
	struct usart_data * data = (struct usart_data *) dev->upriv;
	struct usart_regs * regs = data->regs;

	regs->udr = c;
	return 1;
}

int avr_usart_getc(uartdev_t * dev) {
	struct usart_data * data = (struct usart_data *) dev->upriv;
	struct usart_regs * regs = data->regs;

	return regs->udr;
}

int avr_usart_avail(uartdev_t * dev) {
	struct usart_data * data = (struct usart_data *) dev->upriv;
	struct usart_regs * regs = data->regs;

	return bits_mask(regs->ucsr_a, (1 << RXC0));
}

int avr_usart_room(uartdev_t * dev) {
	struct usart_data * data = (struct usart_data *) dev->upriv;
	struct usart_regs * regs = data->regs;

	return bits_mask(regs->ucsr_a, (1 << UDRE0));
}

int avr_usart_startup(uartdev_t * dev) {
	struct usart_data * data = (struct usart_data *) dev->upriv;
	struct usart_regs * regs = data->regs;
	int8_t irqbase = data->irqbase;
	
	//enable RX
	bits_set(regs->ucsr_b, (1 << RXEN0));
	
	//enable TX
	bits_set(regs->ucsr_b, (1 << TXEN0));

	setIRQHandler(irqbase + 0, (IRQHandler) avr_usart_interrupt_handler_rx, dev);
	setIRQHandler(irqbase + 1, (IRQHandler) avr_usart_interrupt_handler_tx, dev);
	setIRQHandler(irqbase + 2, (IRQHandler) avr_usart_interrupt_handler_tc, dev);
}

int avr_usart_irq_enable_rx(uartdev_t * dev, bool en) { 
	struct usart_data * data = (struct usart_data *) dev->upriv;
	struct usart_regs * regs = data->regs;

	bits_lvl(regs->ucsr_b, (1 << RXCIE0), en);
}

int avr_usart_irq_enable_tx(uartdev_t * dev, bool en) { 
	struct usart_data * data = (struct usart_data *) dev->upriv;
	struct usart_regs * regs = data->regs;

	bits_lvl(regs->ucsr_b, (1 << UDRIE0), en);
}


int avr_usart_irq_enable_tc(uartdev_t * dev, bool en) { 
	struct usart_data * data = (struct usart_data *) dev->upriv;
	struct usart_regs * regs = data->regs;

	bits_lvl(regs->ucsr_b, (1 << TXCIE0), en);
}

	
int avr_usart_setup(uartdev_t * dev, struct uart_params *params) {
	struct usart_data * data = (struct usart_data *) dev->upriv;
	struct usart_regs * regs = data->regs;
	
	int baud = params->baudRate;

	regs->ucsr_a |= 1 << U2X0;
	uint16_t baud_setting = (F_CPU / 4 / baud - 1) / 2;
  
	regs->ubrr_h = baud_setting >> 8;
	regs->ubrr_l = baud_setting;

// PARITY
//	bits_mask_assign_bias(regs->ucsr_c, mode, 0b11, 4);

// STOP BITS
//	bits_mask_assign_bias(regs->ucsr_c, mode, 0b1, 3);

// DATA BITS
//	bits_mask_assign_bias(regs->ucsr_c, mode, 0b011, 1);
//	bits_mask_assign(regs->ucsr_b, mode, 0b100);
}

void avr_usart_interrupt_handler_rx(uartdev_t * dev) {
	struct usart_data * data = (struct usart_data *) dev->upriv;
	struct usart_regs * regs = data->regs;
	dev->uirqops->rx_handler(dev, data->regs->udr);
}

void avr_usart_interrupt_handler_tx(uartdev_t * dev) {
	struct usart_data * data = (struct usart_data *) dev->upriv;
	dev->uirqops->tx_handler(dev);
}

void avr_usart_interrupt_handler_tc(uartdev_t * dev) {
	struct usart_data * data = (struct usart_data *) dev->upriv;
	dev->uirqops->tc_handler(dev);
}

const struct uart_operations avr_usart_ops = {
	.putc = avr_usart_putc,
	.getc = avr_usart_getc,
	.avail = avr_usart_avail,
	.room = avr_usart_room,

	.startup = avr_usart_startup,
	.setup = avr_usart_setup,

	.irq_enable_rx = avr_usart_irq_enable_rx,
	.irq_enable_tx = avr_usart_irq_enable_tx,
	.irq_enable_tc = avr_usart_irq_enable_tc,
};

#endif