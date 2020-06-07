#include <hal/board.h>
#include <systime/systime.h>

#include <hal/irq.h>
#include <util/cpu_delay.h>

int main() 
{
	board_init(0);
	irqs_enable();

	while(1) 
	{
		board::sysled.toggle();
		board::sysuart.sendbyte('a');
		delay(1000);
	}
}
