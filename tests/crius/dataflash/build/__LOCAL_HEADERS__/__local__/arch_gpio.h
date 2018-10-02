#ifndef GENOS_DRIVERS_AVR_GPIO_H
#define GENOS_DRIVERS_AVR_GPIO_H

#include "inttypes.h"

struct gpio {
	volatile uint8_t pin;
	volatile uint8_t ddr;
	volatile uint8_t port;
} __attribute__((packed));

typedef unsigned char __gpio_mask_t;

#if defined (CHIP_ATMEGA2560)
#define GPIOA ((struct gpio *)(0x20))
#define GPIOB ((struct gpio *)(0x23))
#define GPIOC ((struct gpio *)(0x26))
#define GPIOD ((struct gpio *)(0x29))
#define GPIOE ((struct gpio *)(0x2C))
#define GPIOF ((struct gpio *)(0x2F))
#define GPIOG ((struct gpio *)(0x32))
#define GPIOH ((struct gpio *)(0x100))
#define GPIOJ ((struct gpio *)(0x103))
#define GPIOK ((struct gpio *)(0x106))
#define GPIOL ((struct gpio *)(0x109))
#elif defined (CHIP_ATMEGA328P)
#define GPIOB ((struct gpio *)(0x23))
#define GPIOC ((struct gpio *)(0x26))
#define GPIOD ((struct gpio *)(0x29))
#else 
#	error "unrecognized chip"
#endif

#endif