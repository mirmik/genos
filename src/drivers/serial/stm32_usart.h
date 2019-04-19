#ifndef GENOS_DRIVERS_STM32_USART_H
#define GENOS_DRIVERS_STM32_USART_H

#include <drivers/serial/uart.h>
#include <asm/stm32_usart.h>

namespace genos
{
	namespace drivers
	{
		class stm32_usart : public genos::drivers::uart
		{
		public:
			stm32_usart(struct usart_regs * regs, uint8_t irqno) :
				regs(regs), irqno(irqno)
			{}


			int enable(bool en = true) override
			{
				stm32_usart_enable(regs, en);
				return 0;
			}
			
			int txirq(bool en) override
			{
				stm32_usart_txirq_enable(regs, en);
				return 0;
			}
			
			int recvbyte() override
			{
				return stm32_usart_getc(regs);
			}
			
			int sendbyte(int symbol) override
			{
				return stm32_usart_putc(regs, symbol);				
			}
			
			int cantx() override
			{
				return stm32_usart_room(regs);
			}
			
			int hasrx() override
			{
				return stm32_usart_avail(regs);
			}
			
			int setup(int32_t baud     = 9600,
			          char parity      = 'n',
			          uint8_t databits = 8,
			          uint8_t stopbits = 1
			         ) override
			{
				stm32_usart_setup(regs, baud, parity, databits, stopbits);
				txirq(true);
			}
			
		private:
			struct usart_regs * regs;
			uint8_t irqno;
		};
	}
}

#endif