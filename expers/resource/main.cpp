#include <hal/board.h>
#include <genos/sched.h>

#include <drivers/serial/uartring.h>
#include <utility/contty.h>

#include <genos/executor.h>

UARTRING_DECLARE(serial0, "serial0", &usart0, 16, 128);

int hello(int argc, char ** argv) { return 0; }
int hello2(int argc, char ** argv) { return 0; }

SYSCMD(main_shell_table,
	{ "hello", hello, CMDFUNC, "Help here" },
	{ "hello2", hello2, CMDCOOP, "Help2 here" }
);

EXECUTOR(executor, 
	genos::navigation_shell_table,
	main_shell_table
);

int main()
{
	board_init();
	scheduler_init();

	//genos::contty ctty0(&serial0, &executor);

	irqs_enable();

	while (1)
		__schedule__();
}

void __schedule__()
{
	ktimer_manager_step();
	schedee_manager_step();
}