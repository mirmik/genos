#ifndef AVR_USART_OPS_H
#define AVR_USART_OPS_H

#include <periph/usart.h>
#include <gxx/util/bits.h>
#include <drivers/serial/uart_params.h>

static int usart_sendbyte(struct usart_regs* regs, char c) {
	regs->udr = c;
	return 1;
}

static int usart_recvbyte(struct usart_regs* regs) {
	return regs->udr;
}

static int usart_cansend(struct usart_regs* regs) {
	return bits_mask(regs->ucsr_a, (1 << RXC0));
}

static int usart_canrecv(struct usart_regs* regs) {
	return bits_mask(regs->ucsr_a, (1 << UDRE0));
}

static int usart_enable(struct usart_regs* regs, bool en) {}

static int usart_enable_rx(struct usart_regs* regs, bool en) {
	bits_lvl(regs->ucsr_b, (1 << RXEN0), en);
}

static int usart_enable_tx(struct usart_regs* regs, bool en) {
	bits_lvl(regs->ucsr_b, (1 << TXEN0), en);
}

static int usart_enable_irq_rx(struct usart_regs* regs, bool en) {
	bits_lvl(regs->ucsr_b, (1 << RXCIE0), en);
}

static int usart_enable_irq_tx(struct usart_regs* regs, bool en) {
	bits_lvl(regs->ucsr_b, (1 << UDRIE0), en);
}

static int usart_enable_irq_tc(struct usart_regs* regs, bool en) {
	bits_lvl(regs->ucsr_b, (1 << TXCIE0), en);
}

static int usart_setup(
	struct usart_regs* regs, 
	int32_t baud, 
	genos::Uart::Parity parity, 
	genos::Uart::StopBits stopBits, 
	genos::Uart::DataBits dataBits) 
{	
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

static void usart_irq_handlers(int irqbase,
	irq_handler_t irqhrx, void* argrx,
	irq_handler_t irqhtx, void* argtx,
	irq_handler_t irqhtc, void* argtc
	) {
	set_irq_handler(irqbase + 0, irqhrx, argrx);
	set_irq_handler(irqbase + 1, irqhtx, argtx);
	set_irq_handler(irqbase + 2, irqhtc, argtc);
}

#endif