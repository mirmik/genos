#include <hal/board.h>
#include <util/delay.h>

int main() {
	board_init();	

	while(1) 
	{
		gpio_pin_toggle(&board::sysled);
		cpu_delay(100000);
	}
}