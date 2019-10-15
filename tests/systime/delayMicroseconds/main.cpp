#include <hal/board.h>
#include <hal/irq.h>
#include <asm/dwt.h>

#include <systime/systime.h>

int main()
{
	board_init(1);

	// Должен работать при отключенных прерываниях
	irqs_disable();

	while(1) 
	{
		board::sysled.toggle();
		delayMicroseconds(500000);
	}
}