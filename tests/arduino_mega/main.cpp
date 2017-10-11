#include <hal/board.h>

#include <hal/irqs.h>
#include <hal/gpio.h>

#include <genos/systime.h>

//#include <gxx/debug/dprint.h>


int main() {
	board_init();
	hal::irqs::enable();

	auto led = hal::gpio(GPIOB)[7];
	led.mode(hal::gpio::output);

	while(1) {
		systime::delay(500);
		led.tgl();
	}
	
	while(1);

	//hal::gpio::pin(GPIOB, (1<<7)) led;
}