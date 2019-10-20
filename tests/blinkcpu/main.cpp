#include <hal/board.h>
#include <hal/irq.h>

#include <util/cpu_delay.h>

int main()
{
	board_init();

	irqs_enable();

	while(1) 
	{
		cpu_delay(1000000);
		dprln("Hello");

		board::sysled.toggle();
	}
}