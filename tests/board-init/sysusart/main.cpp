#include <hal/board.h>
#include <hal/irq.h>

#include <systime/systime.h>
#include <util/cpu_delay.h>

int main()
{
	board_init();

	dprln("HelloWorld");

	while(1) 
	{
		cpu_delay(500000);
		dprln("HelloWorld");
		board::sysled.toggle();
	}
}