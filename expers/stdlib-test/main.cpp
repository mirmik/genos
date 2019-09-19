#include <hal/board.h>
#include <systime/systime.h>

#include <hal/irq.h>
#include <util/cpu_delay.h>

char a[] = "hello"; 
char b[] = "world";

int main() 
{
	board_init();

	strcpy(a, b);

	irqs_enable();

	while(1) 
	{
		gpio_pin_toggle(&board_led);
		delay(100);
	}
}
