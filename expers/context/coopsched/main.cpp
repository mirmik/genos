#include <hal/board.h>
#include <hal/irq.h>

#include <genos/api.h>

#include <genos/schedee/coop.h>
#include <genos/schedee/autom.h>

#include <genos/sched.h>
#include <genos/ktimer.h>

#include <igris/dprint.h>

#define STACK_SIZE 256

void* task(void* arg) 
{
	(void) arg;
	while(1) 
	{
		dprln("task");
		board::sysled.toggle();
		msleep(1000);
	}
}

char sch_heap[128];
genos::coopschedee sch(task, NULL, sch_heap, 128);

int main() 
{
	board_init();

	dprln("init scheduler");
	scheduler_init();
	
	sch.start();
	
	irqs_enable();

	dprln("start schedule");
	while(1) __schedule__();

}


void __schedule__() 
{
	while(1) {
		ktimer_manager_step();
		schedee_manager_step();
	}
}
