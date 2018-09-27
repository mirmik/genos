#include <hal/board.h>
#include <drivers/gpio/gpio.h>

#include <drivers/spi/avr_spi.h>

int main() {
	board_init();

	gpio_set_level(GPIOB, 1<<7, 1);

	while(1) {
	}
}