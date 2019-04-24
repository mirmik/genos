#include <igris/util/bug.h>
#include <hal/board.h>
#include <hal/irq.h>

#include <systime/systime.h>

#include <drivers/cdev/pipe.h>

void blink_loop() 
{
	debug_print_line("blink_loop");
	
	irqs_enable();
	while(1) 
	{
		gpio_pin_write(&board_led, 1);
		delay(500);
		gpio_pin_write(&board_led, 0);
		delay(500);
	}
}

int main() 
{
	board_init();
	gpio_pin_write(&board_led, 1);

	irqs_enable();

	blink_loop();
}