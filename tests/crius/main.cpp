#include <hal/board.h>

#include <arch/irqs.h>
#include <arch/gpio.h>
#include <arch/uart.h>

#include <genos/systime.h>

#include <drivers/uartring.h>
//
#include <genos/tasklet.h>
//#include <gxx/debug/dprint.h>

char buf[48];
char ibuf[8];
arch::usart u0(usart0_data);
drivers::uartring_ostream serial(&u0, buf);
drivers::uartring_istorage iserial(&u0, ibuf); 

void func();

genos::tasklet tasklet { func };

namespace genos {
	tasklet_manager_singleton tasklet_manager;
}

 void func() {
	arch::gpio::pin led = arch::gpio(GPIOB)[7];
	//led.clr();
 	serial.println("func");
 	while(iserial.avail()) iserial.getchar();
	iserial.set_avail_callback(gxx::make_delegate(&genos::tasklet::plan, &tasklet));
}

int main() {
	board_init();
	
	arch::gpio::pin led = arch::gpio(GPIOB)[7];
	led.mode(hal::gpio::output);

	//u0.setup(115200); 

	serial.init();
	//iserial.init();

	//iserial.set_avail_callback(gxx::make_delegate(&genos::tasklet::plan, &tasklet));

	arch::irqs::enable();
	//u0.enable();

	while(1) {
		genos::tasklet_manager.execute();

		systime::delay(1000);
		serial.println("HelloWorld {}");
		led.tgl();
	}
}