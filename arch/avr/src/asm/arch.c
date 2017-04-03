#include "util/bits.h"
#include "avr/io.h"
#include "avr/interrupt.h"
#include "hal/arch.h"
#include "kernel/diag.h"
//#include "kernel/panic.h"
#include "kernel/irq.h"
//#include <drivers/timer_counter.h>
//#include <periph/timer_device.h>

#include <periph/timer_device.h>

#include <kernel/systime.h>

int usart0_diag_init();
int usart0_diag_putchar(char c);

extern struct diag_ops usart0_diag;

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
	
	UCSR0B|= _BV(RXEN0);
	UCSR0B|= _BV(TXEN0);
};


void arch_init() {	
	irqtable_init();

	usart0_diag.putc = usart0_diag_putchar;
	usart0_diag.write = diag_write_stub;
	current_diag = &usart0_diag;
	usart0_diag_init();

	setIRQHandler(timer0.irqs.ovf, system_tick, 0);
	tc_8bit_interruptOverflowEnable(&timer0, 1);
	tc_8bit_divider(&timer0, 64);
	
//	tc_ovf_int_enable(0, system_tick);
//	tc_start(0, TIMER_DIVIDER_64);
}

void arch_idle() {
	
}

void arch_shutdown(arch_shutdown_mode_t mode) {
	switch(mode)
	{
		case ARCH_SHUTDOWN_MODE_HALT:
		break;
		case ARCH_SHUTDOWN_MODE_REBOOT:
		break;
		case ARCH_SHUTDOWN_MODE_ABORT:
		break;
	};
	global_irq_disable();
	while(1);
}