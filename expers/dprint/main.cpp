#include <hal/board.h>
#include <util/cpu_delay.h>

#include <igris/dprint.h>

int main() 
{
	board_init();

	while(1) 
	{
		board::sysled.toggle();
		dprln("HelloWorld");
		cpu_delay(50000);
	}
}
