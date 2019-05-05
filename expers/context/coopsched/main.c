#include <hal/board.h>
#include <hal/irq.h>

#include <sched/api.h>

#include <sched/sched.h>
#include <sched/timer.h>

#include <igris/dprint.h>

#define STACK_SIZE 256

void* task(void* arg) 
{
	(void) arg;
	while(1) 
	{
		dprln("task");
		gpio_pin_toggle(&board_led);
		msleep(1000);
	}
}

void* autom_task(void* arg, int* state) 
{
	dprln("autom_task");
	gpio_pin_toggle(&board_led);
	msleep(1000);
}

int main() 
{
	board_init();

	dprln("init scheduler");
	scheduler_init();
	
	//struct schedee* sch = create_cooperative_schedee(task, NULL, 128);
	//schedee_run(sch);

	struct schedee* asch = create_autom_schedee(autom_task, NULL);
	schedee_run(asch);

	irqs_enable();

	dprln("start schedule");
	while(1) __schedule__();

}


void __schedule__() 
{
	while(1) {
		timer_manager_step();
		schedee_manager_step();
	}
}
