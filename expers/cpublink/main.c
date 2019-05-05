#include <hal/board.h>
#include <systime/systime.h>

#include <hal/irq.h>
#include <util/delay.h>

int main() 
{
	board_init();

	while(1) 
	{
		gpio_pin_toggle(&board_led);
		cpu_delay(50000);
	}
}