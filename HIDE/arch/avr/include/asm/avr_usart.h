#ifndef AVR2560_USART_H
#define AVR2560_USART_H

#include <avr/io.h>
#include <igris/util/bits.h>

struct usart_regs
{
	volatile uint8_t ucsr_a;
	volatile uint8_t ucsr_b;
	volatile uint8_t ucsr_c;
	volatile uint8_t reserve0;
	volatile uint8_t ubrr_l;
	volatile uint8_t ubrr_h;
	volatile uint8_t udr;
} __attribute__((packed));


static inline int avr_usart_sendbyte(struct usart_regs* regs, char c)
{
	regs->udr = c;
	return 1;
}

static inline int avr_usart_recvbyte(struct usart_regs* regs)
{
	return regs->udr;
}

static inline int avr_usart_cansend(struct usart_regs* regs)
{
	return bits_mask(regs->ucsr_a, (1 << RXC0));
}

static inline int avr_usart_canrecv(struct usart_regs* regs)
{
	return bits_mask(regs->ucsr_a, (1 << UDRE0));
}

static inline void avr_usart_enable_rx(struct usart_regs* regs, int en)
{
	bits_lvl(regs->ucsr_b, (1 << RXEN0), en);
}

static inline void avr_usart_enable_tx(struct usart_regs* regs, int en)
{
	bits_lvl(regs->ucsr_b, (1 << TXEN0), en);
}

static inline void avr_usart_rxirq(struct usart_regs* regs, int en)
{
	bits_lvl(regs->ucsr_b, (1 << RXCIE0), en);
}

static inline void avr_usart_txirq(struct usart_regs* regs, int en)
{
	bits_lvl(regs->ucsr_b, (1 << UDRIE0), en);
}

static inline void avr_usart_tcirq(struct usart_regs* regs, int en)
{
	bits_lvl(regs->ucsr_b, (1 << TXCIE0), en);
}

static inline void avr_usart_setup(
    struct usart_regs* regs,
    uint32_t baud,
    char parity,
    uint8_t databits,
    uint8_t stopbits
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