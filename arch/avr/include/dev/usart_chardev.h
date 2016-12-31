#ifndef GENOS_AVR_USART_CHARDEV_H
#define GENOS_AVR_USART_CHARDEV_H

#include <periph/regs/usart.h>
#include <drivers/serial/uart.h>

namespace periph {

	class Usart : public Uart {
	public:
		volatile usart_regs* regs;
		usart_irq_nums* irqs;

	public:
		Usart(volatile usart_regs* _regs, const usart_irq_nums* _irqs) : regs(_regs), irqs((usart_irq_nums*)_irqs) {};

		int putc(char c);
		int getc();

		bool avail();
		bool ready();
	
		void setRXHandler(Kernel::IRQHandler handler);
		void setTXHandler(Kernel::IRQHandler handler);

		void enableRXInt(bool b);
		void enableTXInt(bool b);

		void enableRX(bool b);
		void enableTX(bool b);

		void settings(const UartParams& params);
	};

	extern Usart usart0;
	extern Usart usart1;
};

#endif