#include <avr/io.h>
#include <hal/board.h>

#include <drivers/gpio/avr_gpio.h>

avr_gpio_pin led(GPIOB, (1<<7));

int main() 
{
	board_init();
	board_test();
}