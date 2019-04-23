#include <hal/board.h>
#include <hal/irq.h>

#include <asm/arch.h>
#include <asm/rcc.h>

#include <drivers/gpio/gpio.h>

#include <periph/map.h>
#include <periph/irqdefs.h>

#include <util/delay.h>
#include <systime/systime.h>
#include <igris/dprint.h>

#include <drivers/serial/uart.h>
#include <drivers/serial/stm32_usart.h>
#include <drivers/serial/uartring.h>

#include <sched/sched.h>
#include <sched/timer.h>
#include <sched/schedee/autom.h>
#include <sched/schedee/cooperative.h>

#include <utility/contty.h>
#include <utility/mshell.h>

#include <banner.h>

void blink(void * arg, struct ktimer * timer) { 
	gpio_pin_toggle(&board_led); 
	ktimer_replan(timer);
}

UARTRING_DECLARE(serial2, &usart2, 16, 128);
KTIMER_DECLARE(blink_timer, blink, NULL, 500);

CONTTY_CONTEXT(contty_cntxt, &serial2.cdev);
//struct contty_context contty_cntxt;
struct autom_schedee contty_schedee;

char init_schedee_heap[512];
struct cooperative_schedee init_schedee;

int hello(int argc, char** argv) 
{
	dprln("Hello");
}

int banner(int argc, char** argv) 
{
	debug_print_banner();
}

struct mshell_command mshell_commands_table [] = {
	{"hello", hello, MSHELL_FUNCTION},
	{"banner", banner, MSHELL_FUNCTION},
	MSHELL_TBLFIN
};

void* mainproc(void* arg) 
{

} 

int main() 
{
	board_init();

	uart_device_setup(&usart2, 115200, 'n', 8, 1);
	uartring_begin(&serial2, &usart2.dev);

	gpio_pin_settings(&board_led, GPIO_MODE_OUTPUT);
	gpio_pin_write(&board_led, 1);

	scheduler_init();
	ktimer_plan(&blink_timer);

	//contty_debug_mode(&contty_cntxt, true);
	
	autom_schedee_init(&contty_schedee, contty_automate, &contty_cntxt);
	cooperative_schedee_init(&init_schedee, mainproc, NULL, init_schedee_heap, 512);
	schedee_run(&contty_schedee.sch);

	irqs_enable();

	while(1) 
	{
		__schedule__();
	}
}

void __schedule__() 
{
	timer_manager_step();
	schedee_manager_step();
}