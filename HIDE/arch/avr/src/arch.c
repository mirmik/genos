#include <stddef.h>
#include <hal/arch.h>
#include <hal/irqtable.h>

#include <asm/avr_timer.h>
#include <systime/systime.h>

#include <diag/diag.h>
#include <periph/map.h>

__BEGIN_DECLS

extern struct diag_ops usart0_diag;
int usart0_diag_init();	

void arch_init() 
{	
	irqtable_init();

	usart0_diag_init();
	diag_setup(&usart0_diag, NULL);

	irqtable_set_handler(ATMEGA_IRQ_TIM0_OVF, system_tick, NULL);
	avr_timer8_ovfirq_enable(TIMER0, 1);
	avr_timer8_divider(TIMER0, 64);

	systime_set_frequency(F_CPU / 256 / 64);
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
	irqs_disable();
	while(1);
}

__END_DECLS