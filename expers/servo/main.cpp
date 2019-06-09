#include <hal/board.h>

#include <systime/systime.h>
#include <drivers/pwmservo/pwmservo.h>
#include <addons/arduino/pwm.h>

int main() 
{
	board_init();

	ARDUINO_PWMS[8];

	while(1) 
	{
		gpio_pin_toggle(&board_led);
		delay(200);
	}
}