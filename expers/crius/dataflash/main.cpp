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
#include <drivers/bdev/atmel_dataflash.h>

#include <string.h>

avr_spi_device spi;

gpio_pin dataflash_selector( GPIOB, 1<<0 );
gpio_pin dataflash_reset( GPIOG, 1<<0 );

gpio_pin red_led(RED_LED_GPIO, RED_LED_MASK);

void led_blink_timer(void* arg, struct ktimer * tim) {
	gpio_toggle(RED_LED_GPIO, RED_LED_MASK);
	gpio_toggle(GREEN_LED_GPIO, GREEN_LED_MASK);
	gpio_toggle(YELLOW_LED_GPIO, YELLOW_LED_MASK);

	ktimer_swift(tim);
	ktimer_plan(tim);
}

int main() {
	uint8_t sts;

	board_init();
	schedee_manager_init();

	gpio_settings(RED_LED_GPIO, RED_LED_MASK, GPIO_MODE_OUTPUT);
	gpio_settings(GREEN_LED_GPIO, GREEN_LED_MASK, GPIO_MODE_OUTPUT);
	gpio_settings(YELLOW_LED_GPIO, YELLOW_LED_MASK, GPIO_MODE_OUTPUT);

	ktimer_create_for(led_blink_timer, NULL, 1000);

	spi.init("spi");
	spi.enable();

	irqs_enable();

 	const char * senddata = "\xF1\xF2lloWorld";
 	char recvdata[20];
	//memset(data, 0, 20);

	AT45DB041 dataflash(&spi, &dataflash_selector, &dataflash_reset);

	dataflash.reset();
	dataflash.lock();

	sts = dataflash.get_status();
	dprhexln(sts & 0x3C);

	sts = dataflash.get_status();
	dprhexln((sts & 0x3C) >> 2);

	sts = dataflash.is_ready();	
	dprhexln(sts);

	sts = dataflash.is_ready();	
	dprhexln(sts);

	delay(10);

	sts = dataflash.is_ready();	
	dprhexln(sts);

	sts = dataflash.is_ready();	
	dprhexln(sts);

	dataflash.flashpage_to_buffer2(3);
	dataflash.read_from_buffer(2, 10, (uint8_t*)recvdata, 10);
	dprdump(recvdata, 10);

	//dataflash.flashread(3, 10, (uint8_t*)recvdata, 10);
	dprdump(recvdata, 10);

	dataflash.unlock();

	__schedule__();
}

void __schedule__() {
	while(1) {
		timer_manager();
		schedee_manager();
	}
}