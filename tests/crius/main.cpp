#include <hal/board.h>

#include <arch/irqs.h>
#include <arch/gpio.h>
#include <arch/uart.h>

#include <genos/systime.h>

#include <drivers/uartring.h>
//
#include <genos/tasklet.h>
#include <genos/timer.h>
//#include <gxx/debug/dprint.h>

/*char buf[48];
char ibuf[8];
arch::usart u0(usart0_data);
drivers::uartring_ostream serial(&u0, buf);
drivers::uartring_istorage iserial(&u0, ibuf); 

void reader_func();
genos::tasklet reader_tasklet { reader_func };
void reader_func() {
	while(iserial.avail()) iserial.getchar();
	iserial.set_avail_callback(reader_tasklet.make_plan_delegate());
}*/

arch::gpio::pin rled(GPIOB, 7);
arch::gpio::pin gled(GPIOC, 7);
arch::gpio::pin yled(GPIOC, 6);

genos::timer rblink_timer ( gxx::make_delegate(&arch::gpio::pin::tgl, &rled), 1000, 0 );
genos::timer gblink_timer ( gxx::make_delegate(&arch::gpio::pin::tgl, &gled), 500, 0 );
genos::timer yblink_timer ( gxx::make_delegate(&arch::gpio::pin::tgl, &yled), 250, 0 );

int main() {
	board_init();
	
	rled.mode(hal::gpio::output);
	gled.mode(hal::gpio::output);
	yled.mode(hal::gpio::output);

	//u0.setup(115200); 

	//serial.init();
	//iserial.init();
	//iserial.set_avail_callback(reader_tasklet.make_plan_delegate());

	//u0.enable();
	arch::irqs::enable();

	rblink_timer.autorepeat(true).plan();
	gblink_timer.autorepeat(true).plan();
	yblink_timer.autorepeat(true).plan();

	while(1) {
//		genos::tasklet_manager.execute();
		genos::timer_manager.execute();
	}
}