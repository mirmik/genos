#include <hal/board.h>
#include <genos/sched.h>

#include <drivers/serial/uartring.h>
#include <utility/contty.h>

UARTRING_DECLARE(serial0, "serial0", &usart0, 16, 128);

int main()
{
	board_init();
	scheduler_init();

	genos::contty ctty0(&serial0);

	irqs_enable();

	while (1)
		__schedule__();
}

void __schedule__()
{
	ktimer_manager_step();
	schedee_manager_step();
}