#include <hal/board.h>

#include <arch/irqs.h>
#include <arch/gpio.h>
#include <arch/uart.h>

#include <genos/systime.h>

#include <drivers/uartring.h>

#include <genos/tasklet.h>

//#include <gxx/debug/dprint.h>

char buf[48];
char ibuf[8];
arch::usart u0(usart0_data);
drivers::uartring_ostream serial(&u0, buf);
drivers::uartring_istorage iserial(&u0, ibuf); 

void func() {
	dprln("func");
}

genos::tasklet tasklet { func };
genos::tasklet_manager tmanager;

int main() {
	board_init();
	
	auto led = arch::gpio(GPIOB)[7];
	led.mode(hal::gpio::output);

	u0.setup(115200); 
	u0.enable_tx();
	u0.enable_rx();

	//iserial.set_avail_callback(gxx::make_delegate(func));

	serial.init();
	iserial.init();

	tmanager.plan(tasklet);

	arch::irqs::enable();

	while(1) {
		tmanager.execute();

		systime::delay(1000);
		serial.println("HelloWorld");
		led.tgl();
	}
	
	while(1);
	//hal::gpio::pin(GPIOB, (1<<7)) led;
}