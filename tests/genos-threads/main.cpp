#include <hal/board.h>
#include <systime/systime.h>
#include <genos/sched.h>
#include <genos/api.h>
#include <genos/schedee/coop.h>
#include <genos/schedee/autom.h>

void * thread0_function(void*) 
{
	dprln("thread0_0");
	msleep(100);
	dprln("thread0_1");
	msleep(100);
	dprln("thread0_2");
	msleep(100);
	return 0;
}

void * thread1_function(void*) 
{
	dprln("thread1_0");
	msleep(100);
	dprln("thread1_1");
	msleep(100);
	dprln("thread1_2");
	msleep(100);
	return 0;
}

void * blink_function(void * foo, int * state) 
{
	board::sysled.toggle();
	msleep(200);
	return 0;
}

genos::autom_schedee blink(blink_function, nullptr);

char stack0[512];
genos::coop_schedee thread0(thread0_function, nullptr, stack0, 512);

char stack1[512];
genos::coop_schedee thread1(thread1_function, nullptr, stack1, 512);

int main() 
{
	board_init();
	schedee_manager_init();
	irqs_enable();

	dprln("system inited");

	/*while(1) 
	{
		delay(500);
		board::sysled.toggle();
	}*/

	blink.start();
	thread0.start();
	thread1.start();

	while(1)
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