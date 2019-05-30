#include <hal/board.h>
#include <genos/sched.h>
#include <genos/ktimer.h>

#include <genos/mvfs/vfsnode.h>
#include <genos/mvfs/directory.h>

#include <genos/schedee/autom.h>

#include <drivers/serial/uartring.h>
#include <drivers/cdev/virtual/debug.h>

#include <unistd.h>

#include  <genos/ktimer.h>

UARTRING_DECLARE(serial0, "serial0", &usart0, 16, 128);
int wr;

void* task(void* priv, int* state) 
{
	int ans;
	dprln("state:", *state);

	switch(*state) 
	{
		case 0:
			dprln("TASK");
			++*state;
			ans = genos::open_node(&serial0);
			dprln(ans);
			break;

		case 1:
			++*state;
			//write(wr, "HelloWorld", 10);
			break;

		case 2:
			schedee_exit();
			break;
	}
}

FDTABLE(sch_restbl, 5);

genos::directory devdir("dev");
genos::directory mntdir("mnt");

genos::debug_device dbgdev;


void blink(void* arg, genos::ktimer* tim) 
{
	gpio_pin_toggle(&board_led);
	tim->replan();
}

autom_schedee sch(task, nullptr);
genos::ktimer blink_timer(blink, NULL, 1000);

int main() 
{
	board_init();
	scheduler_init();

	genos::root_directory.add_child(&devdir);
	genos::root_directory.add_child(&mntdir);
	devdir.add_child(&dbgdev);

	sch.set_fdtable(sch_restbl, 5);
	sch.run();

	blink_timer.plan();

	irqs_enable();

	while(1) 
		__schedule__();
}

void __schedule__() 
{
	ktimer_manager_step();
	schedee_manager_step();
}