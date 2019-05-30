#include <hal/board.h>
#include <genos/sched.h>
#include <genos/ktimer.h>
#include <genos/vfs.h>
#include <genos/schedee/autom.h>

#include <drivers/serial/uartring.h>
#include <drivers/cdev/virtual/debug.h>

#include <unistd.h>

autom_schedee sch;

UARTRING_DECLARE(serial0, "serial0", &usart0, 16, 128);
int wr;

void* task(void* priv, int* state) 
{
	switch(*state) 
	{
		case 0:
			wr = genos::open_resource(&serial0);
			*state++;
			break;

		case 1:
			*state++;
			write(wr, "HelloWorld", 10);

		case 2:
			schedee_exit();
			break;
	}
}

RESOURCE_TABLE(sch_restbl, 5);

genos::directory devdir("dev");
genos::directory mntdir("mnt");

genos::debug_device dbgdev;

int main() 
{
	board_init();
	scheduler_init();

	genos::root_directory.add_child(&devdir);
	genos::root_directory.add_child(&mntdir);
	devdir.add_child(&dbgdev);

	sch.init(task, nullptr);
	sch.set_resource_table(sch_restbl, 5);
	sch.run();

	while(1) 
		__schedule__();
}

void __schedule__() 
{
	ktimer_manager_step();
	schedee_manager_step();
}