#include <genos/hal/board.h>

#include <arch/irqs.h>
#include <arch/gpio.h>
#include <arch/uart.h>

#include <genos/systime.h>
#include <drivers/uartring.h>
//
#include <genos/tasklet.h>
#include <genos/timer.h>
#include <genos/schedule.h>
#include <genos/sched/schedee.h>
#include <gxx/terminal/terminal.h>
//#include <gxx/packager/packager.h>

#include <gxx/gstuff/automate.h>
#include <genos/banner.h>

//#include <genos/event/waiter.h>

#include <arch/i2c_automate.h>

char serbuf[512], iserbuf[32];
arch::usart u0(usart0_data);
drivers::uartring serial(&u0, serbuf, iserbuf);

arch::i2c_automate i2c_autom;

arch::gpio::pin rled(GPIOB, 7);
arch::gpio::pin gled(GPIOC, 7);
arch::gpio::pin yled(GPIOC, 6);

genos::timer gblink_timer ( gxx::make_delegate(&arch::gpio::pin::tgl, &gled), 1000, 0 );
genos::timer yblink_timer ( gxx::make_delegate(&arch::gpio::pin::tgl, &yled), 500, 0 );
genos::timer rblink_timer ( gxx::make_delegate(&arch::gpio::pin::tgl, &rled), 250, 0 );

char termbuf[100];
gxx::gstuff::automate pack(termbuf);

//gxx::terminal_core term {serial, termbuf};

/*void terminal_func(); genos::tasklet terminal_tasklet { terminal_func };
void terminal_func() {
	while(serial.avail()) term.newchar(serial.getchar()); 
	serial.set_avail_callback(terminal_tasklet.make_plan_delegate());
}*/

void terminal_func(); 
genos::tasklet terminal_tasklet { terminal_func };

void terminal_func() {
	while(serial.avail()) { 
		char c = serial.getchar();
		pack.newchar(c);  
		//dprln("term:", c, (int)c);
	}
	//serial.set_avail_callback(terminal_tasklet.make_plan_delegate());
	serial.avail_flag.wait();
}

#include <gxx/serialize/serialize.h>
void package_dump(gxx::buffer buf) {
	gxx::archive::binary_reader archive(buf);

	gxx::buffer command;
	int32_t a;
	int32_t b;	
	int32_t c;	
	int32_t d;	

	gxx::deserialize(archive, command);
	gxx::deserialize(archive, a);
	gxx::deserialize(archive, b);
	gxx::deserialize(archive, c);
	gxx::deserialize(archive, d);

	dprln(command);
	dprln(a);
	dprln(b);
	dprln(c);
	dprln(d);

	dprln("result:", a + b);
} 

void unplan() {
	rblink_timer.unbind();
	gblink_timer.unbind();
	yblink_timer.unbind();
	rled.clr();
	gled.clr();
	yled.clr();

}

void plan() {
	rblink_timer.plan();
	gblink_timer.plan();
	yblink_timer.plan();
}

void system_execute(gxx::buffer line) {
	dprln(line);
	if (line == "on") {
		plan();
	}
	if (line == "off") {
		unplan();
	}
}

void i2c_finish(uint8_t mode) {
	dprln("i2c_finish");
}

int main() {
	board_init();

	void* ptr = malloc(33);
	pack.set_callback(package_dump);
	pack.debug_mode(true);
	
	rled.mode(hal::gpio::output);
	gled.mode(hal::gpio::output);
	yled.mode(hal::gpio::output);

	rled.set();
	gled.set();
	yled.set();

	u0.setup(115200); 

	serial.init();
	serial.avail_flag.set_handler(terminal_tasklet.make_plan_delegate());

	u0.enable();

	//term.init();
	//term.line_handler = system_execute;

	arch::irqs::enable();
	//u0.sendbyte('a');

	/*while(true) {
		systime::delay(1000);
		vt.putchar('f');
		vt.del();
		vt.down(1);
	}*/

	i2c_autom.init_master(100000);
	i2c_autom.enable();

	i2c_autom.set_master_finish_handler(i2c_finish);

	genos::print_banner(serial);
	genos::print_about(serial);
	//term.start();

	rblink_timer.autorepeat(true);
	gblink_timer.autorepeat(true);
	yblink_timer.autorepeat(true);
	plan();

	while(1) {
		genos::schedule();
	}
}

void genos::schedule() {
	genos::tasklet_manager.execute();
	genos::timer_manager.execute();
	//genos::schedee_manager.execute();
}