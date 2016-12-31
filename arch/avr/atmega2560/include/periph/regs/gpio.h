#ifndef AVR2560_GPIO_H
#define AVR2560_GPIO_H

#include "hal/reg.h"

struct gpio_regs {
	reg_t pin;
	reg_t ddr;
	reg_t port;
} __attribute__((packed));

typedef unsigned char gpio_mask_t;

#define GPIO_PORT_BASE_A 0x20
#define GPIO_PORT_BASE_B 0x23
#define GPIO_PORT_BASE_C 0x26
#define GPIO_PORT_BASE_D 0x29
#define GPIO_PORT_BASE_E 0x2C
#define GPIO_PORT_BASE_F 0x2F
#define GPIO_PORT_BASE_G 0x32

#define GPIOA ((struct gpio_regs *)(GPIO_PORT_BASE_A))
#define GPIOB ((struct gpio_regs *)(GPIO_PORT_BASE_B))
#define GPIOC ((struct gpio_regs *)(GPIO_PORT_BASE_C))
#define GPIOD ((struct gpio_regs *)(GPIO_PORT_BASE_D))

#endif