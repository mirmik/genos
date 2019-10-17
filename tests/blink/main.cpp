#include <hal/board.h>
#include <hal/irq.h>

#include <systime/systime.h>

#include <asm/gpio.h>
#include <asm/rcc.h>

#include <util/cpu_delay.h>

int main()
{
	//arch_init();
	board_init();

	irqs_enable();

	while(1) 
	{
		delay(100);
		dprln(millis());

		board::sysled.toggle();
	}
}