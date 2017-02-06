#ifndef AVR2560_GPIO_H
#define AVR2560_GPIO_H

#include "inttypes.h"

struct gpio_regs {
	volatile uint8_t pin;
	volatile uint8_t ddr;
	volatile uint8_t port;
} __attribute__((packed));

typedef unsigned char gpio_mask_t;

#define GPIO_PORT_BASE_A 0x20
#define GPIO_PORT_BASE_B 0x23
#define GPIO_PORT_BASE_C 0x26
#define GPIO_PORT_BASE_D 0x29
#define GPIO_PORT_BASE_E 0x2C
#define GPIO_PORT_BASE_F 0x2F
#define GPIO_PORT_BASE_G 0x32
#define GPIO_PORT_BASE_H 0x100
#define GPIO_PORT_BASE_J 0x103
#define GPIO_PORT_BASE_K 0x106
#define GPIO_PORT_BASE_L 0x109

#define GPIOA ((struct gpio_regs *)(GPIO_PORT_BASE_A))
#define GPIOB ((struct gpio_regs *)(GPIO_PORT_BASE_B))
#define GPIOC ((struct gpio_regs *)(GPIO_PORT_BASE_C))
#define GPIOD ((struct gpio_regs *)(GPIO_PORT_BASE_D))
#define GPIOE ((struct gpio_regs *)(GPIO_PORT_BASE_E))
#define GPIOF ((struct gpio_regs *)(GPIO_PORT_BASE_F))
#define GPIOG ((struct gpio_regs *)(GPIO_PORT_BASE_G))
#define GPIOH ((struct gpio_regs *)(GPIO_PORT_BASE_H))
#define GPIOJ ((struct gpio_regs *)(GPIO_PORT_BASE_J))
#define GPIOK ((struct gpio_regs *)(GPIO_PORT_BASE_K))
#define GPIOL ((struct gpio_regs *)(GPIO_PORT_BASE_L))

struct gpio_pin_s {
	struct gpio_regs* regs;
	uint8_t offset;
};

static const struct gpio_pin_s gpio_table[] =
{
	{GPIOE, 0}, //0 //RX0
	{GPIOE, 1}, //1 //TX0	
	{GPIOE, 4}, //2 	//OC3B
	{GPIOE, 5}, //3	//OC3C
	{GPIOG, 5}, //4	//OC0B
	{GPIOE, 3}, //5	//OC3A
	{GPIOH, 3}, //6	//OC4A
	{GPIOH, 4}, //7	//OC4B
	{GPIOH, 5}, //8	//OC4C
	{GPIOH, 6}, //9	//OC2B
	{GPIOB, 4}, //10	//OC2A
	{GPIOB, 5}, //11	//OC1A
	{GPIOB, 6}, //12	//OC1B
	{GPIOB, 7}, //13	//0C0A / OC1C //RED_LED
	{GPIOJ, 1}, //14 //TX3
	{GPIOJ, 0}, //15 //RX3
	{GPIOH, 1}, //16 //TX2
	{GPIOH, 0}, //17 //RX2
	{GPIOD, 3}, //18 //TX1 
	{GPIOD, 2}, //19 //RX1
	{GPIOD, 1}, //20 //SDA
	{GPIOD, 0}, //21 //SCL
	{GPIOA, 0}, //22
	{GPIOA, 1}, //23
	{GPIOA, 2}, //24
	{GPIOA, 3}, //25
	{GPIOA, 4}, //26
	{GPIOA, 5}, //27
	{GPIOA, 6}, //28
	{GPIOA, 7}, //29
	{GPIOC, 7}, //30		//AIOP : GREEN
	{GPIOC, 6}, //31		//AIOP : YELLOW
	{GPIOC, 5}, //32
	{GPIOC, 4}, //33
	{GPIOC, 3}, //34
	{GPIOC, 2}, //35
	{GPIOC, 1}, //36
	{GPIOC, 0}, //37
	{GPIOD, 7}, //38
	{GPIOG, 2}, //39
	{GPIOG, 1}, //40 		
	{GPIOG, 0}, //41 		
	{GPIOL, 7}, //42
	{GPIOL, 6}, //43
	{GPIOL, 5}, //44	//OC5C
	{GPIOL, 4}, //45	//OC5B
	{GPIOL, 3}, //46	//OC5A
	{GPIOL, 2}, //47
	{GPIOL, 1}, //48
	{GPIOL, 0}, //49
	{GPIOB, 3}, //50
	{GPIOB, 2}, //51
	{GPIOB, 1}, //52
	{GPIOB, 0}, //53
};

#endif