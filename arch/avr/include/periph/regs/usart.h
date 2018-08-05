#ifndef AVR2560_USART_H
#define AVR2560_USART_H

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

#endif 