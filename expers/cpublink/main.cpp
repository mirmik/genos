#include <hal/board.h>
#include <systime/systime.h>

#include <hal/irq.h>
#include <util/cpu_delay.h>

int main() 
{
	board_init();

	while(1) 
	{
		board::sysled.toggle();
		cpu_delay(500000);
	}
}
