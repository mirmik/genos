#include <hal/board.h>
#include <hal/irq.h>

#include <systime/systime.h>

int main()
{
	board_init();

	dprln("HelloWorld");

	board::sysuart.setup(115200, 'n', 8, 1);

	irqs_enable();

	while(1) 
	{
		delay(500);
		dprln("HelloWorld");
		board::sysled.toggle();
	}
}