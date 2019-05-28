#include <hal/board.h>
#include <hal/irq.h>

#include <systime/systime.h>

//#include <drivers/timer/avr_timer.h>
#include <igris/dprint.h>

int main() 
{
	board_init();

	irqs_enable();

	//periph::timer1.set_divider

	while(1) 
	{
		delay(500);
		gpio_pin_toggle(&board_led);
	}
}