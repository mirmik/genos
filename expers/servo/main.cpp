#include <hal/board.h>

#include <systime/systime.h>
#include <drivers/pwmservo/pwmservo.h>
#include <addons/arduino/pwm.h>
#include <addons/arduino/pin.h>

#include <math.h>

uint16_t maxocr;
genos::avr::pwmservo pwm11(GET_ARDUINO_PWM(11));
genos::avr::pwmservo_writer<uint16_t> wr;

int main() 
{
	board_init();

	pinMode(11, 1);

	pwm11.standart_timer_mode();
	pwm11.pwm_mode_start();
	wr = pwm11.get_writer(1000, 2000);

	wr.set(0.5);

	irqs_enable();

	while(1) 
	{
		float val = 0.5 + sin((float)(millis()) / 1000) * 0.5;
		wr.set(val);
		dprln(val);
		delay(1);
	}
}