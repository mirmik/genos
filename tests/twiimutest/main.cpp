#include <genos/hal/board.h>
#include <genos/hal/gpio.h>
#include <genos/hal/irqs.h>

#include <genos/schedule.h>
#include <genos/sched/tasklet.h>
#include <genos/sched/timer.h>
#include <genos/sched/schedee.h>

#include <genos/hal/uart.h>
#include <arch/usart.h>

#include <genos/drivers/uartring.h>

void ledtoggle() { board::led.tgl(); }

char rxserbuf[16], txserbuf[64];
arch::usart usart0(usart0_data);
drivers::uartring serial(&usart0, rxserbuf, txserbuf);

int main() {
	board_init();

	serial.init();
	(new genos::timer_tasklet(ledtoggle, 1000))->autorepeat(true).plan();

	gxx::println_to(serial, "HelloWorld");

	genos::hal::irqs::enable();
	while(1) genos::schedule();
}

namespace genos {
	void schedule() {
		//genos::tasklet_manager.exec();
		genos::timer_manager.exec();
		//genos::schedee_manager.exec();
	}
}