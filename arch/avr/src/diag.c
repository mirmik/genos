#include <hal/arch.h>
#include <hal/irq.h>
#include <avr/io.h>

#include <gxx/diag/diag.h>

#include <avr/io.h>

int usart0_diag_putchar(void* _, char c)
{
	irqstate_t save = irqs_save();
	UCSR0A |= 1<<TXC0;
	while ((UCSR0A & (1 << UDRE0)) == 0) {};  
	UDR0=c; 
	//while ((UCSR0A & (1 << UDRE0)) == 0) {}; 
	while ((UCSR0A & (1 << TXC0)) == 0) {}; 
	irqs_restore(save);

	return 0;
}


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

	return 0;
}

const struct diag_ops usart0_diag = {
	.putc = usart0_diag_putchar,
	.getc = NULL,
	.write = diag_write_stub,
	.read = NULL
};

				
		