	.syntax unified
	.cpu cortex-m4
	.fpu softvfp
	.thumb

#define RCC_BASE        0x40023800
#define RCC_AHB1ENR     RCC_BASE + 0x30 
#define GPIOD_BASE      0x40020C00
#define GPIOD_MODER     GPIOD_BASE
#define GPIOD_ODR       GPIOD_BASE + 0x14

#define GPIODEN         1 << 3
#define MODER15_OUT     1 << 30
#define MODER14_OUT     1 << 28
#define MODER13_OUT     1 << 26
#define MODER12_OUT     1 << 24
#define LED_BLUE        1 << 15
#define LED_RED         1 << 14
#define LED_ORANGE      1 << 13
#define LED_GREEN       1 << 12
#define DELAY           0x000F

.global memcpy
.global main
.global end_of_programm
.global cxx_invoke_init_array
.global cxx_invoke_fini_array

.section    .text
.global __start
.type     __start, %function

__start:

	/* copy data section */
    ldr     r0, =_data_virtual_address
    ldr     r1, =_data_load_address
    ldr     r2, =_data_size
    bl	    memcpy

    /* init array invoke */
    bl cxx_invoke_init_array

    /*user program*/
    bl 		main

    /* fini array invoke */
    bl cxx_invoke_fini_array

    /*return from main function*/
    bl end_of_programm