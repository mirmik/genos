#include <hal/board.h>
#include <genos/dev/uartring.h>

#include <hal/irqs.h>
#include <genos/iolib.h>

#include <genos/banner.h>

char serbuf[16], iserbuf[16];

arch::usart u0(usart0_data);
genos::uartring serial(&u0, serbuf, iserbuf);

//#include <drivers/uartring.h>

//#include <arch/irqs.h>
//#include <arch/i2c_automate.h>

//#include <genos/schedule.h>

/*char serbuf[512], iserbuf[32];

char i2c_buffer[128];
arch::i2c_automate i2c_autom(i2c_buffer);

void i2c_finish(uint8_t mode) {
	dprln("i2c_finish");
}
*/
int main() {
	board_init();

	//u0.enable();

	serial.init();


//	i2c_autom.init_master(100000);
//	i2c_autom.enable();

//	i2c_autom.set_master_finish_handler(i2c_finish);

	genos::hal::irqs::enable();
	serial.write("Mirmik", 6, 0);

	genos::writeall(&serial, "01234567890123456789012345678901234567890123456789", 32);
	genos::writeall(&serial, "\n\r", 2);

	//int arg = 1000;
	//i2c_autom.start_write(0x32, gxx::buffer::on_object(arg));

//	i2c_autom.start(0x32);
//	i2c_autom.print(arg);
//	i2c_autom.flush();

	genos::print_banner(&serial);
	genos::print_about(&serial);
	while(1) {
//		genos::schedule();
	}
}
/*
void genos::schedule() {
	//genos::tasklet_manager.execute();
	//genos::timer_manager.execute();
	//genos::schedee_manager.execute();
}*/