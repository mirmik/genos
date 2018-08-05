#include <genos/hal/board.h>
#include <genos/hal/gpio.h>
#include <genos/hal/irqs.h>

#include <genos/schedule.h>
#include <genos/sched/tasklet.h>
#include <genos/sched/timer.h>
#include <genos/sched/schedee.h>

#include <g1/tower.h>
#include <g1/indexes.h>
#include <g0/services/echo.h>
#include <genos/drivers/g1/uartgate.h>

#include <genos/hal/uart.h>
#include <arch/usart.h>

#include <gxx/sshell.h>

//#include <genos/drivers/uartring.h>
#include <g1/kits/spammer.h>

int i = 0;

void spammer() {
	gxx::fprintln("HelloWorld {0}", i++);
}

void g1_incoming_handler(g1::packet* pack) {
	if (pack->header.type == G1_G0TYPE) {
		g0::travell(pack);
	} else {
		g1::release(pack);
	}
}

int ledfunc(int argc, const char* argv[]) {
	gxx::println("ledfunc");
	if (argc < 3) return -1;
	if (!strcmp(argv[1], "on")) {
		gxx::println("ledfunc on");
		switch(atoi(argv[2])) {
			case 0: board::red_led.set(); break;
			case 1: board::yellow_led.set(); break;
			case 2: board::green_led.set(); break;
		}
	}
	else if (!strcmp(argv[1], "off")) {
		gxx::println("ledfunc off");
		switch(atoi(argv[2])) {
			case 0: board::red_led.clr(); break;
			case 1: board::yellow_led.clr(); break;
			case 2: board::green_led.clr(); break;
		}
	}
}

arch::usart usart0(usart0_data);
genos::uartgate usart0gate(&usart0, 128);

uint8_t outspam_addr[8];
g1::dgramm_spammer output(".42.12.127.0.0.1:10009", outspam_addr, 8);

gxx::sshell shell;

gxx::sshell::command shelltable[] = {
	{"led", ledfunc},
};

void console(g0::message* msg) {
	g0::utilize(msg);
	shell.execute(msg->data, msg->size);
}

int main() {
	board_init();
	g1::incoming_handler = g1_incoming_handler;

	g1::link_gate(&usart0gate, 42);
	g0::make_service(21, console);

	usart0.setup(115200);
	usart0gate.init();
	usart0.enable();

	shell.settable(shelltable, sizeof(shelltable) / sizeof(gxx::sshell::command));

	gxx::standart_output = &output;
	(new genos::timer_tasklet(spammer, 1000))->autorepeat(true).plan();

	genos::hal::irqs::enable();
	while(1) genos::schedule();
}

namespace g1 {
	uint16_t millis() {
		return systime::millis();
	}
}

namespace genos {
	void schedule() {
		g1::onestep();
		genos::tasklet_manager.exec();
		genos::timer_manager.exec();
		genos::schedee_manager.exec();
	}
}