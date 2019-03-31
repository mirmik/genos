#include <hal/board.h>
#include <systime/systime.h>
#include <util/delay.h>

#include <nos/print.h>

#include <drivers/serial/uartring.h>

genos::uartring serial0;
char serial0_txbuf[128];
char serial0_rxbuf[16];

int main() {
	board_init();
	board::usart0.setup(115200);
	serial0.init(&board::usart0, serial0_rxbuf, 16, serial0_txbuf, 128);
	
	irqs_enable();

	while(1) 
	{
		serial0.println("HelloWorld");
		board::sysled.toggle();
		systime::delay(1000);
	}
}