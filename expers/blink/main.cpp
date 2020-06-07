#include <hal/board.h>
#include <systime/systime.h>

#include <hal/irq.h>

int main() 
{
	board_init(0);
	irqs_enable();

	while(1) 
	{
		board::sysled.toggle();
		delay(1000);
	}
}