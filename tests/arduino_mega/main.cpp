#include <hal/board.h>
#include <hal/irq.h>
#include <gxx/debug/dprint.h>

#include <drivers/gpio.h>
#include <genos/systime.h>

#include <hal/irqs.h>
#include <hal/gpio.h>

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