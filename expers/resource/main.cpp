#include <hal/board.h>
#include <genos/sched.h>

#include <drivers/serial/uartring.h>
#include <utility/contty.h>

#include <genos/executor.h>
#include <genos/api.h>
#include <genos/schedee/coop.h>

UARTRING_DECLARE(serial2, "serial2", &board::sysusart, 16, 128);

int hello(int argc, char ** argv) { dprln("HELLOW"); return 0; }
int hello2(int argc, char ** argv) { dprln("HELLOW");return 0; }

SYSCMD(main_shell_table,
	{ "hello", hello, CMDFUNC, "Help here" },
	{ "hello2", hello2, CMDFUNC, "Help2 here" }
);

EXECUTOR(executor, 
	genos::navigation_shell_table,
	main_shell_table
);

genos::contty ctty2(&serial2, &executor);

void* blink(void*, int*state) 
{
	gpio_pin_toggle(&board_led);
	msleep(200);
	return nullptr;
}
genos::autom_schedee blsch(blink, nullptr);
	
int main()
{
	board_init();
	scheduler_init();

	board::sysusart.setup(115200, 'n', 8, 1);
	serial2.begin(&board::sysusart);

	//ctty2.debug_mode = true;
	//serial2.debug_mode = true;
	
	ctty2.run();
	blsch.run();

	irqs_enable();

	while (1)
		__schedule__();
}

void __schedule__()
{
	while(1) 
	{
		ktimer_manager_step();
		schedee_manager_step();
	}
}