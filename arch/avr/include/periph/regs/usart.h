#ifndef AVR2560_USART_H
#define AVR2560_USART_H

#include <gxx/util/bits.h>

struct usart_regs {
	volatile uint8_t ucsr_a;
	volatile uint8_t ucsr_b;
	volatile uint8_t ucsr_c;
	volatile uint8_t reserve0;
	volatile uint8_t ubrr_l;
	volatile uint8_t ubrr_h;
	volatile uint8_t udr;
} __attribute__((packed));

#if defined (CHIP_ATMEGA2560)
#define USART0 ((struct usart_regs *)(0xC0))
#define USART1 ((struct usart_regs *)(0xC8))
#define USART2 ((struct usart_regs *)(0xD0))
#elif defined (CHIP_ATMEGA328P)
#define USART0 ((struct usart_regs *)(0xC0))
#else 
#	error "unrecognized chip"
#endif

static int usart_regs_sendbyte(struct usart_regs* regs, char c) {
	regs->udr = c;
	return 1;
}

static int usart_regs_recvbyte(struct usart_regs* regs) {
	return regs->udr;
}

static int usart_regs_cansend(struct usart_regs* regs) {
	return bits_mask(regs->ucsr_a, (1 << RXC0));
}

static int usart_regs_canrecv(struct usart_regs* regs) {
	return bits_mask(regs->ucsr_a, (1 << UDRE0));
}

static int usart_regs_enable_rx(struct usart_regs* regs, bool en) {
	bits_lvl(regs->ucsr_b, (1 << RXEN0), en);
}

static int usart_regs_enable_tx(struct usart_regs* regs, bool en) {
	bits_lvl(regs->ucsr_b, (1 << TXEN0), en);
}

static int usart_regs_rxirq(struct usart_regs* regs, bool en) {
	bits_lvl(regs->ucsr_b, (1 << RXCIE0), en);
}

static int usart_regs_txirq(struct usart_regs* regs, bool en) {
	bits_lvl(regs->ucsr_b, (1 << UDRIE0), en);
}

static int usart_regs_tcirq(struct usart_regs* regs, bool en) {
	bits_lvl(regs->ucsr_b, (1 << TXCIE0), en);
}

#include <drivers/serial/settings.h>

static int usart_regs_setup(
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

#endif 