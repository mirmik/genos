#include <hal/board.h>
#include <util/delay.h>

int main() {
	board_init();	

	while(1) 
	{
		board::sysled.toggle();
		cpu_delay(100000);
	}
}