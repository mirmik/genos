#include <hal/board.h>
#include <drivers/gpio/gpio.h>
#include <drivers/gpio/pin.h>

#include <drivers/spi/avr_spi.h>

GPIO_PIN(dataflash_pin, GPIOB, 6);

int main() {
	char buf[128] = "\0\x29";

	board_init();

	struct spi_device * spi = get_avr_spi_device();

	spi_select(spi, &dataflash_pin, 1);
	spi_exchange(spi, buf, buf, 2, NOSCHED);

	gpio_set_level(GPIOB, 1<<7, 1);

	while(1) {
	}
}