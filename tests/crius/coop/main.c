#include <hal/board.h>
#include <hal/irq.h>
#include <drivers/gpio/gpio.h>
#include <drivers/gpio/pin.h>

#include <gxx/debug/dprint.h>
#include <gxx/debug/delay.h>

#include <sched/sched.h>
#include <sched/timer.h>

#include <sched/api.h>

#include <gxx/util/iteration_counter.h>

int i;

void led_blink_timer(void* arg, struct ktimer * tim) {
	gpio_toggle(RED_LED_GPIO, RED_LED_MASK);
	gpio_toggle(GREEN_LED_GPIO, GREEN_LED_MASK);
	gpio_toggle(YELLOW_LED_GPIO, YELLOW_LED_MASK);

	ktimer_swift(tim);
	ktimer_plan(tim);
}

void * mproc(void * arg) {
	void* ptr = arg;
	int r = __atomic_is_lock_free(0, ptr);
	dprdec(r);

	debug_print_line("HelloWorld");
	__displace__();
	debug_print_line("HelloWorld");

	while (1) {
		do_iteration(3) {
			debug_print_line("msleep");
			msleep(1000);
		}
		do_after {
			debug_print_line("mproc exit");
			schedee_exit(0);
		};
	}

	return NULL;
}

int main() {
	board_init();
	schedee_manager_init();

	gpio_settings(RED_LED_GPIO, RED_LED_MASK, GPIO_MODE_OUTPUT);
	gpio_settings(GREEN_LED_GPIO, GREEN_LED_MASK, GPIO_MODE_OUTPUT);
	gpio_settings(YELLOW_LED_GPIO, YELLOW_LED_MASK, GPIO_MODE_OUTPUT);

	ktimer_create_for(led_blink_timer, NULL, 1000);
	schedee_run(create_cooperative_schedee(mproc, NULL, 256));

	irqs_enable();
	__schedule__();
}

void __schedule__() {
	while(1) {
		timer_manager();
		schedee_manager();
	}
}