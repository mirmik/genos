#ifndef GENOS_DRIVERS_STM32_USART_H
#define GENOS_DRIVERS_STM32_USART_H

#include <drivers/serial/uart.h>
#include <periph/irqdefs.h>
#include <asm/stm32_usart.h>
#include <asm/nvic.h>

#include <hal/irqtable.h>

#include <drivers/gpio/pin.h>

namespace genos 
{
	struct stm32_usart : public genos::uart
	{
		USART_TypeDef * regs;
		uint8_t irqno;
	
		int setup(int32_t baud, char parity, uint8_t databits, uint8_t stopbits) override;
		int enable(int en) override;
		int ctrirqs(uint8_t cmd) override;
		int recvbyte() override;
		int sendbyte(int c) override;
		int cantx() override;
		int hasrx() override;
	
		void irqinit();
	
		stm32_usart(USART_TypeDef * regs, uint8_t irqno) :
			regs(regs), irqno(irqno) 
		{}

		void init_gpio(gpio_pin tx, gpio_pin rx, int af);
	};
}

#endif