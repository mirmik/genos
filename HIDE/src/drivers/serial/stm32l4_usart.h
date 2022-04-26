#ifndef GENOS_DRIVERS_STM32_USART_H
#define GENOS_DRIVERS_STM32_USART_H

#include <drivers/serial/uart.h>
#include <periph/irqdefs.h>
#include <asm/usart.h>
#include <asm/nvic.h>

#include <hal/irqtable.h>

struct stm32l4_usart_device : public uart_device
{
	usart_regs_t * regs;
	uint8_t irqno;

	int setup(int32_t baud, char parity, uint8_t databits, uint8_t stopbits) override;
	int enable(int en) override;
	int ctrirqs(uint8_t cmd) override;
	int recvbyte() override;
	int sendbyte(int c) override;
	int cantx() override;
	int hasrx() override;

	void irqinit();

	stm32l4_usart_device(usart_regs_t * regs, uint8_t irqno) :
		regs(regs), irqno(irqno) 
	{}
};

#define STM32_USART_DEVICE_DECLARE(name, regs, irqno)	\
struct stm32_usart_device name {regs, irqno}

#endif