#include "genos/hal/arch.h"
#include "genos/hal/irq.h"
#include "avr/io.h"

#include <gxx/diag/diag.h>

#include <avr/io.h>

int usart0_diag_putchar(void* _, char c)
{
	irqstate_t save = global_irqs_save();
	UCSR0A |= 1<<TXC0;
	while ((UCSR0A & (1 << UDRE0)) == 0) {};  
	UDR0=c; 
	//while ((UCSR0A & (1 << UDRE0)) == 0) {}; 
	while ((UCSR0A & (1 << TXC0)) == 0) {}; 
	global_irqs_restore(save);
};


#define SERIAL_8N1 0x06
int usart0_diag_init() {	
	UCSR0A = 0;
	uint16_t baud_setting = (F_CPU / 8 / 115200 - 1) / 2;
	
	// assign the baud_setting, a.k.a. ubrr (USART Baud Rate Register)
	UBRR0H = baud_setting >> 8;
	UBRR0L = baud_setting;
	
	uint8_t config = SERIAL_8N1;
	//set the data bits, parity, and stop bits
	#if defined(__AVR_ATmega8__)
		config |= 0x80; // select UCSRC register (shared with UBRRH)
	#endif
	UCSR0C = config;
	
	//UCSR0B|= _BV(RXEN0);
	UCSR0B |= 1 << TXEN0;
};

struct diag_ops usart0_diag;

				
		