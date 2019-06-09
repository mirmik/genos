#include <hal/board.h>

#include <systime/systime.h>
#include <drivers/pwmservo/pwmservo.h>
#include <addons/arduino/pwm.h>

genos::avr::pwmservo pwm11(ARDUINO_PWM[11]);

int main() 
{
	board_init();

	pwm11.standart_timer_mode();

	irqs_enable();

	while(1) 
	{
		dprln(pwm11.tim->value16());
		gpio_pin_toggle(&board_led);
		delay(200);
	}
}