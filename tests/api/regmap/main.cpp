#include <hal/board.h>
#include <hal/irq.h>

#include <util/cpu_delay.h>
#include <drivers/regmap.h>

#include <drivers/spi/stm32-spi.h>

int main()
{
	board_init();

	genos::stm32_spi_device spi0(SPI1, 0);

	while(1) 
	{
		cpu_delay(100000);
		board::sysled.toggle();
	}
}