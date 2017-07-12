#include <hal/board.h>
#include <hal/irq.h>
#include <drivers/gpio.h>

//#include <drivers/serial/avr/UsartDevice.h>
#include <gxx/debug/dprint.h>
#include <gxx/debug/delay.h>

//gxx::array<char, 128> txbuf, rxbuf;
//UsartDevice usart0(usart0_data, txbuf, rxbuf);

#include <vector>
#include <string>

std::vector<int> vec;
std::string str;

void setup();
void loop();

int main() {
	board_init();

	setup();
	global_irqs_enable();
	while(1) loop();
}

void setup() {
	gpio_settings(GPIOB, (1<<7), GPIO_MODE_OUTPUT);
	gpio_set_level(GPIOB, (1<<7), 1);
	
	vec.push_back(3);

//	usart0.begin(115200);
}

void loop() {
	debug_delay(1000);
	debug_print("HelloWorld\r\n");
	gpio_tgl_level(GPIOB, (1<<7));
}