#include <hal/board.h>
#include <gxx/debug/dprint.h>
//#include <drivers/gpiotbl.h>

//#include <gxx/array.h>
//#include <array>
//#include <genos/banner.h>

/*#include <console/Terminal.h>
#include <kernel/devices/serial/Stream.h>
#include <kernel/devices/serial/PipeStream.h>

#include <drivers/serial/avr/UsartStream.h>

#include <console/SimpleShell.h>*/

//Genos::DebugExecutor debugExecutor;
//Genos::SimpleShell shell; 

//Genos::DebugStream debugStream;
//Genos::PipeStream pipeStream((new gxx::array<char, 256>)->slice());

//std::array<char, 256> rxbuf, txbuf;

//Genos::AvrUsartStream io(usart0_data, rxbuf.slice(), txbuf.slice());
//Genos::Terminal terminal(&io, &io, &shell, (new gxx::array<char, 64>)->slice());

/*int hello(int argc, char** argv) {
	dprln("HelloWorld");
	return Genos::SimpleShell::RetCodeOK;
}

int ledon(int argc, char** argv) {
	pinnum_set_level(13,1);
	return Genos::SimpleShell::RetCodeOK;
}

int ledoff(int argc, char** argv) {
	pinnum_set_level(13,0);
	return Genos::SimpleShell::RetCodeOK;
}*/

int main() {
	board_init();

/*	debug_print("HELLOWORLD");

	io.begin(115200);
	global_irq_enable();

	//debugStream.println("HelloWorld");
	print_banner(io);
	print_about(io);

	shell.add("hello", hello);
	shell.add("on", ledon);
	shell.add("off", ledoff);

	//terminal.debug_mode = true;
	terminal.echo(true).run();

	while(1) {
		Genos::Glue::schedule();
	}*/
}