#include <genos/hal/board.h>
#include <genos/hal/irqs.h>

#include <genos/systime.h>
#include <drivers/uartring.h>

int main() {
	board_init();
	arch::irqs::enable();

	dprln("HelloWorld");	
	board::i2c.init();
	board::i2c.enable();

	char _buf[48] = "1234"; 
	gxx::buffer buf(_buf, 4);
	board::i2c.write(0x47, buf);

	while(1) {
		systime::delay(1000);
		board::led.tgl();
	}
}