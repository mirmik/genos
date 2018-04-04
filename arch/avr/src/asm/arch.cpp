#include <avr/io.h>
#include <avr/interrupt.h>

#include <genos/hal/arch.h>
#include <genos/hal/irqtbl.h>
#include <genos/hal/irqs.h>

#include <gxx/diag/diag.h>


#include <periph/timer_device.h>

#include <genos/systime.h>

__BEGIN_DECLS
int usart0_diag_init();
int usart0_diag_putchar(char c);
__END_DECLS

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
	
	//UCSR0B|= _BV(RXEN0);
	//UCSR0B|= _BV(TXEN0);
};


void arch_init() {	
	genos::hal::irqtbl::init();

	usart0_diag.putc = usart0_diag_putchar;
	usart0_diag.write = diag_write_stub;
	current_diag_ops = &usart0_diag;
	usart0_diag_init();

	genos::hal::irqtbl::set_handler(timer0.irqs.ovf, gxx::make_fastaction(systime::system_tick));
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
	arch::irqs::disable();
	while(1);
}