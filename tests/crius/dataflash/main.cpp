#include <hal/board.h>
#include <hal/irq.h>
#include <drivers/gpio/gpio.h>
#include <drivers/gpio/pin.h>

#include <gxx/debug/dprint.h>
#include <gxx/debug/delay.h>

#include <sched/sched.h>
#include <sched/timer.h>

#include <sched/api.h>

#include <drivers/spi/avr_spi.h>

avr_spi_device spi;

void led_blink_timer(void* arg, struct ktimer * tim) {
	gpio_toggle(RED_LED_GPIO, RED_LED_MASK);
	gpio_toggle(GREEN_LED_GPIO, GREEN_LED_MASK);
	gpio_toggle(YELLOW_LED_GPIO, YELLOW_LED_MASK);

	ktimer_swift(tim);
	ktimer_plan(tim);
}

int main() {
	board_init();
	schedee_manager_init();

	gpio_settings(RED_LED_GPIO, RED_LED_MASK, GPIO_MODE_OUTPUT);
	gpio_settings(GREEN_LED_GPIO, GREEN_LED_MASK, GPIO_MODE_OUTPUT);
	gpio_settings(YELLOW_LED_GPIO, YELLOW_LED_MASK, GPIO_MODE_OUTPUT);

	ktimer_create_for(led_blink_timer, NULL, 1000);

	spi.init("spi");

	irqs_enable();
	__schedule__();
}

void __schedule__() {
	while(1) {
		timer_manager();
		schedee_manager();
	}
}