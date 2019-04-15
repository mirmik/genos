#include <hal/board.h>
#include <hal/arch.h>
#include "curboard.h"

#include <drivers/gpio/gpio.h>

void board_init() 
{
	arch_init();
}