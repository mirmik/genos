#ifndef AVR_ASM_USART_H
#define AVR_ASM_USART_H

#include <periph/regs/avr/usart.h>

static inline int usart_regs_sendbyte(struct usart_regs* regs, char c)
{
	regs->udr = c;
	return 1;
}

static inline int usart_regs_recvbyte(struct usart_regs* regs)
{
	return regs->udr;
}

static inline int usart_regs_cansend(struct usart_regs* regs)
{
	return bits_mask(regs->ucsr_a, (1 << RXC0));
}

static inline int usart_regs_canrecv(struct usart_regs* regs)
{
	return bits_mask(regs->ucsr_a, (1 << UDRE0));
}

static inline void usart_regs_enable_rx(struct usart_regs* regs, bool en)
{
	bits_lvl(regs->ucsr_b, (1 << RXEN0), en);
}

static inline void usart_regs_enable_tx(struct usart_regs* regs, bool en)
{
	bits_lvl(regs->ucsr_b, (1 << TXEN0), en);
}

static inline void usart_regs_rxirq(struct usart_regs* regs, bool en)
{
	bits_lvl(regs->ucsr_b, (1 << RXCIE0), en);
}

static inline void usart_regs_txirq(struct usart_regs* regs, bool en)
{
	bits_lvl(regs->ucsr_b, (1 << UDRIE0), en);
}

static inline void usart_regs_tcirq(struct usart_regs* regs, bool en)
{
	bits_lvl(regs->ucsr_b, (1 << TXCIE0), en);
}

static inline void usart_regs_setup(
    struct usart_regs* regs,
//	const struct uart_params * s
    int32_t baud,
    char parity,
    uint8_t stopBits,
    uint8_t dataBits
)
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