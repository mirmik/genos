#ifndef SERIAL_H
#define SERIAL_H

#include <zillot/common/uartring.h>
#include <zillot/stm32/usart.h>

extern zillot::uartring uring_rs232;
extern zillot::uartring uring_console;
extern zillot::stm32::usart usart_rs232;
extern zillot::stm32::usart usart_console;

void serial_interfaces_init();

#endif