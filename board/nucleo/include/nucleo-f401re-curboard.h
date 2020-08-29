#ifndef NUCLEO_BOARDS
#define NUCLEO_BOARDS

#   define BOARD_HSE_FREQ 8000000
#	define SYSLED_GPIO GPIOA
#	define SYSLED_MASK (1<<5)

#	define SYSUART USART2
#	define SYSUART_IRQ USART2_IRQn
#	define SYSUART_RX_GPIO GPIOA
#	define SYSUART_TX_GPIO GPIOA
#	define SYSUART_RX_PIN 2
#	define SYSUART_TX_PIN 3
#	define SYSUART_TX_AF 7
#	define SYSUART_RX_AF 7

/*#	define ARDUART USART2
#	define ARDUART_IRQ USART2_IRQn
#	define ARDUART_RX_GPIO GPIOA
#	define ARDUART_TX_GPIO GPIOA
#	define ARDUART_RX_PIN 2
#	define ARDUART_TX_PIN 3
#	define ARDUART_TX_AF 7
#	define ARDUART_RX_AF 7*/

static
const struct gpio_pin PINOUT[] = 
{
	//CN9
	{ GPIOA, 1<<3 },	//d0
	{ GPIOA, 1<<2 },	//d1
	{ GPIOA, 1<<10 },	//d2
	{ GPIOB, 1<<3 },	//d3
	{ GPIOB, 1<<5 },	//d4
	{ GPIOB, 1<<4 },	//d5
	{ GPIOB, 1<<10 },	//d6
	{ GPIOA, 1<<8 },	//d7

	//CN5
	{ GPIOA, 1<<9 },	//d8
	{ GPIOC, 1<<7 },	//d9
	{ GPIOB, 1<<6 },	//d10
	{ GPIOA, 1<<7 },	//d11
	{ GPIOA, 1<<6 },	//d12
	{ GPIOA, 1<<5 }, 	//d13
	{ GPIOB, 1<<9 },	//d14
	{ GPIOB, 1<<8 },	//d15
};

static
const struct gpio_pin PINOUT_A[] = 
{
	// CN8
	{GPIOA, 1<<0}, //a0
	{GPIOA, 1<<1}, //a1
	{GPIOA, 1<<4}, //a2
	{GPIOB, 1<<0}, //a3
	{GPIOC, 1<<1}, //a4
	{GPIOC, 1<<0}  //a5
};

#endif