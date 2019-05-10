#include <hal/board.h>
#include <systime/systime.h>

#include <hal/irq.h>
#include <util/cpu_delay.h>

#include <genos/ktimer.h>

#include <sched/sched.h>
#include <sched/schedee/autom.h>
#include <sched/schedee/cooperative.h>
#include <sched/api.h>

#include <drivers/serial/uartring.h>

#include <utility/contty2.h>
#include <utility/mshell.h>

int schinfo(int argc, char** argv);

void blink(void* arg, struct ktimer * tim) 
{
	gpio_pin_toggle(&board_led);
	ktimer_replan(tim);
}

void infor(void* arg, struct ktimer * tim) 
{
	schinfo(0,NULL);
	ktimer_replan(tim);
}

KTIMER_DECLARE(blink_timer, blink, NULL, 500);
KTIMER_DECLARE(infor_timer, infor, NULL, 1000);
UARTRING_DECLARE(serial2, &usart2.dev, 32, 256);

struct cooperative_schedee sch0;
struct cooperative_schedee sch1;

char sch0_heap[512];
char sch1_heap[512];

void* task0(void* arg) 
{
	while(1)
		__displace__();
}

void* task1(void* arg) 
{
	while(1)
		__displace__();	
}

CONTTY2_DECLARE(contty_cntxt, &serial2.cdev);

struct autom_schedee contty_schedee;

int main() 
{
	board_init();
	scheduler_init();

	uart_device_setup(&usart2.dev, 115200, 'n', 8, 1);
	uartring_begin(&serial2, &usart2.dev);

	ktimer_plan(&blink_timer);

	cooperative_schedee_init(&sch0, task0, NULL, sch0_heap, 512);	
	cooperative_schedee_init(&sch1, task1, NULL, sch1_heap, 512);
	autom_schedee_init(&contty_schedee, contty2_automate, &contty_cntxt);

	schedee_run(&sch0.sch);
	schedee_run(&contty_schedee.sch);

	irqs_enable();

	dprln(sizeof(struct schedee));

	while(1) 
		__schedule__();
}

int hello(int argc, char** argv) 
{
	dprln("HelloWorld");
	return 0;
}

#if SCHEDEE_DEBUG_STRUCT
int schinfo(int argc, char** argv) 
{
	schedee_list_debug_info();
	schedee_manager_debug_info();
	return 0;
}
#endif

struct mshell_command mshell_commands_table[] = 
{
	{ "hello", hello, MSHELL_FUNCTION, "Print 'helloworld'" },

#if SCHEDEE_DEBUG_STRUCT
	{ "schinfo", schinfo, MSHELL_FUNCTION, "scheduler information" },
#endif

	MSHELL_TBLFIN,
};

void __schedule__() 
{
	while(1) 
	{
		timer_manager_step();
		schedee_manager_step();
	}
}