#include <hal/board.h>
#include <hal/irq.h>

#include <systime/systime.h>

#include <igris/dprint.h>

#include <util/delay.h>

int main()
{
	board_init();

	dprln("HelloWorld");

	irqs_enable();

	gpio_pin_write(&board_led, 0);

	while (1)
	{
		delay(500);
		gpio_pin_toggle(&board_led);
	}
}