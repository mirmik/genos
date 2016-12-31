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

void arch_init() {	
	irqtable_init();

	usart0_diag.init = usart0_diag_init;
	usart0_diag.putc = usart0_diag_putchar;
	usart0_diag.write = diag_write_stub;
	current_diag = &usart0_diag;

	diag_init();

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
	ipl_disable();
	while(1);
}