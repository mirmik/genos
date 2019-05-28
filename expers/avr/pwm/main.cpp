#include <hal/board.h>
#include <hal/irq.h>

#include <systime/systime.h>

#include <drivers/timer/avr_timer.h>
#include <igris/dprint.h>

int main() 
{
	board_init();

	periph::timer1.set_divider(64);
	periph::timer1.set_mode(decltype(periph::timer1)::TimerMode::PWM8);

	periph::timer1.set_compare_b(0xFF / 4);
	periph::timer1.set_output_b_mode(0b10);

	gpio_settings(GPIOB, (1<<2), GPIO_MODE_OUTPUT);

	irqs_enable();

	while(1) 
	{
		dprln(periph::timer1.value());
		//delay(500);
		//gpio_pin_toggle(&board_led);
	}
}