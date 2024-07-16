//#include <asm/irqtable.h>
#include <asm/avr_timer.h>
//#include <systime/systime.h>
#include <zillot/diag.h>
#include <periph/map.h>
#include <stddef.h>

__BEGIN_DECLS

extern struct diag_ops usart0_diag;
int usart0_diag_init();	

#include <avr/io.h>
#include <avr/interrupt.h>
#include <igris/time/jiffies-systime.h>

/*ISR (TIMER0_OVF_vect)
{
	jiffies_increment();
}*/

void arch_init() 
{	
	usart0_diag_init();
	diag_setup(&usart0_diag, NULL);

//	irqtable_init();
//	irqtable_set_handler(ATMEGA_IRQ_TIM0_OVF, system_tick, NULL);

	avr_timer8_ovfirq_enable(TIMER0, 1);
	avr_timer8_divider(TIMER0, 64);
	systime_set_frequency(F_CPU / 256 / 64);
}

__END_DECLS