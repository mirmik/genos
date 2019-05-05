#include <hal/board.h>
#include <systime/systime.h>

#include <hal/irq.h>

#include <igris/dprint.h>

int main() 
{
	board_init();

	dprln("enable irqs");
	irqs_enable();

	dprln("loop:");
	while(1) 
	{
		gpio_pin_toggle(&board_led);
		delay(500);
	}
}