#include <hal/board.h>
#include <hal/irq.h>
#include <drivers/gpio/gpio.h>
#include <drivers/gpio/pin.h>

#include <drivers/spi/avr_spi.h>

#include <gxx/debug/dprint.h>
#include <gxx/debug/delay.h>

#include <mvfs/mvfs.h>
#include <mvfs/fstype.h>
#include <mvfs/fsops.h>
#include <mvfs/fs/joke/joke.h>

#include <sched/sched.h>
#include <sched/timer.h>

#include <gxx/util/iteration_counter.h>

GPIO_PIN(dataflash_pin, GPIOB, 6);

int i;

void led_blink_timer(void* arg, struct ktimer * tim) {
	gpio_toggle(GPIOB, 1<<7);

	dprdec(i++);dln();

	do_iteration(7) {
		ktimer_swift(tim);
		ktimer_plan(tim);
	} do_after {
		ktimer_dealloc(tim);
	}
}

int main() {
	__debug_delay_multiplier = 100;

	//char buf[128] = "\0\x29";

	board_init();
	gpio_settings(GPIOB, 1<<7, GPIO_MODE_OUTPUT);

	//struct spi_device * spi = get_avr_spi_device();

	//spi_select(spi, &dataflash_pin, 1);
	
	//int sts = spi_exchange(spi, buf, buf, 2, NOSCHED);
	//if (sts) dprf("exchange status: %d", sts);

	//vfs_register_fs(&joke_fstype);
	//vfs_mount_first("joke", 0, NULL);

	
	//schedee_run(create_autom_schedee(led_blink_task, NULL));

	debug_print_line("HelloWorld");
	ktimer_create_for(led_blink_timer, NULL, 1000);

	irqs_enable();
	__schedule__();
}

void __schedule__() {
	while(1) {
		timer_manager();
		//schedee_manager();
	}
}