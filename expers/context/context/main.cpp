#include <igris/util/bug.h>
#include <hal/board.h>
#include <hal/subst.h>
#include <hal/irq.h>

#include <systime/systime.h>

#define STACK_SIZE 256

struct context cntxt0;
struct context cntxt1;

char stack0[STACK_SIZE];
char stack1[STACK_SIZE];

void blink_loop() 
{
	debug_print_line("blink_loop");
	
	irqs_enable();
	while(1) 
	{
		gpio_pin_write(&board_led, 1);
		delay(500);
		gpio_pin_write(&board_led, 0);
		delay(500);
	}
}

void func0(void* arg) 
{
	debug_print_line("func0");
	context_switch(&cntxt0, &cntxt1);

	debug_print_line("func0_1");
	context_switch(&cntxt0, &cntxt1);

	debug_print_line("func0_2");
	context_switch(&cntxt0, &cntxt1);

	debug_print_line("func0_3");
	blink_loop();
}

void func1(void* arg) 
{
	debug_print_line("func1");
	context_switch(&cntxt1, &cntxt0);

	debug_print_line("func1_1");
	context_save_and_displace(&cntxt1);
	
	debug_print_line("func1_2");
	context_switch(&cntxt1, &cntxt0);

	debug_print_line("func1_3");
	blink_loop();
}

int main() 
{
	board_init();

	gpio_pin_write(&board_led, 1);

	context_init(&cntxt0, (uintptr_t)(stack0 + STACK_SIZE - 1),
		func0, NULL, false);

	context_init(&cntxt1, (uintptr_t)(stack1 + STACK_SIZE - 1),
		func1, NULL, false);

	context_load(&cntxt0);

	irqs_enable();

	debug_print_line("error_main_branch");
	blink_loop();
}

extern "C" void __context_displace_vector__();
void __context_displace_vector__()
{
	debug_print_line("__context_displace_vector__");
	context_load(&cntxt0);
	blink_loop();
}
