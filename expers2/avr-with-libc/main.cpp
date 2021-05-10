#include <avr/io.h>
#include <hal/board.h>

#include <drivers/gpio/avr_gpio.h>
#include <drivers/serial/avr_usart.h>

avr_gpio_pin led(GPIOB, (1<<7));

//DECLARE_AVR_USART_WITH_IRQS(usart0, USART0);
//ISR(USART0_TX_vect);

avr_usart_device usart0(USART0);
__DECLARE_ISR_RX(USART0_RX_vect, usart0);

int main() 
{
	board_init();
	board_test();

	//usart0;
}