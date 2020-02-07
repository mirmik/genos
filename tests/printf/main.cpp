#include <hal/board.h>
#include <hal/irq.h>

#include <math.h>

#include <util/cpu_delay.h>

int main()
{
	board_init();

	dprf("i 30: %d\r\n", 30);
	dprf("f 0.12: %f\r\n", (float)0.12);
	//dprf("f 10.12: %f\r\n", 10.12);
	//dprf("f 10.0: %f\r\n", 10.0);

	while(1) 
	{
		cpu_delay(500000);
	}
}