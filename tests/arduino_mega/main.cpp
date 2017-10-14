#include <hal/board.h>

#include <arch/irqs.h>
#include <arch/gpio.h>
#include <arch/uart.h>

#include <genos/systime.h>

#include <drivers/uartring.h>


//#include <gxx/debug/dprint.h>


int main() {
	board_init();
	
	arch::irqs::enable();
	auto led = arch::gpio(GPIOB)[7];
	led.mode(hal::gpio::output);

	arch::usart u0(usart0_data);

	u0.setup(115200); 
	u0.enable_tx();

	drivers::uartring serial(&u0, gxx::allocate_buffer(48));

	while(1) {
		systime::delay(500);
		u0.sendbyte('a');
		led.tgl();
	}
	
	while(1);
	//hal::gpio::pin(GPIOB, (1<<7)) led;
}