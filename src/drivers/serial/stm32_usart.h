#ifndef GENOS_DRIVERS_STM32_USART_H
#define GENOS_DRIVERS_STM32_USART_H

#include <drivers/serial/uart.h>
#include <asm/usart.h>
#include <asm/nvic.h>

#include <hal/irqtable.h>

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

			int ctrirqs(uint8_t cmd) override
			{
				switch (cmd)
				{
					case UART_CTRIRQS_TXON: stm32_usart_txirq_enable(regs, true); return 0;

					case UART_CTRIRQS_TXOFF: stm32_usart_txirq_enable(regs, false); return 0;

					case UART_CTRIRQS_RXON: stm32_usart_rxirq_enable(regs, true); return 0;

					case UART_CTRIRQS_RXOFF: stm32_usart_rxirq_enable(regs, false); return 0;

					case UART_CTRIRQS_TCON: stm32_usart_tcirq_enable(regs, true); return 0;

					case UART_CTRIRQS_TCOFF: stm32_usart_tcirq_enable(regs, false); return 0;
				}

				return -1;
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
				irqinit();
				nvic_enable_irq(irqno - STM32_IRQ_FIRST);
			}

			static void _irqhandler(void* priv)
			{
				stm32_usart * dev = (stm32_usart *) priv;

				if (stm32_rxirq_status(dev->regs))
					dev->handler(dev->handarg, UART_IRQCODE_RX);

				else if (stm32_txirq_status(dev->regs))
					dev->handler(dev->handarg, UART_IRQCODE_TX);

				else if (stm32_tcirq_status(dev->regs))
					dev->handler(dev->handarg, UART_IRQCODE_TC);

				else
					BUG();
			}

			void irqinit()
			{
				irqtable_set_handler(irqno, &_irqhandler, this);
			}

		private:
			struct usart_regs * regs;
			uint8_t irqno;
		};
	}
}

#endif