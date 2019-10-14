#include <hal/board.h>
#include <hal/irq.h>
#include <asm/dwt.h>

#include <systime/systime.h>

int main()
{
	board_init();
	arm_dwt_init();

	irqs_enable();

	while(1) 
	{
		board::sysled.toggle();
		delayMicroseconds(1000000);
		//delay(100);
	}
}