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

struct diag_ops usart0_diag;

				
		