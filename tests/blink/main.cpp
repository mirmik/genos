#include <hal/board.h>
#include <hal/irq.h>

#include <systime/systime.h>
#include <util/cpu_delay.h>

int main()
{
	board_init();

	while(1) 
	{
		cpu_delay(500000);
		board::sysled.toggle();
	}
}