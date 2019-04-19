#ifndef GENOS_DRIVERS_AVR_UART
#define GENOS_DRIVERS_AVR_UART

#include <drivers/serial/uart.h>
#include <periph/regs/usart.h>

#include <hal/irqtable.h>

namespace genos
{
	namespace drivers
	{
		class avr_usart : public uart
		{
		public:
			int enable(bool en = true) override;
			int ctrirqs(uint8_t cmd) override;
			int recvbyte() override;
			int sendbyte(int symbol) override;
			int cantx() override;
			int hasrx() override;
			int setup(int32_t baud,
			          char parity = 'n',
			          uint8_t databits = 8,
			          uint8_t stopbits = 1) override;

			void init(struct usart_regs * regs, int base_irq) { this->regs = regs; this->base_irq = base_irq; }

			avr_usart(struct usart_regs * regs, int base_irq) : regs(regs), base_irq(base_irq) { }
			avr_usart() {}

		private:
			int irqinit();

			struct usart_regs * regs;
			uint8_t base_irq;

		private:
			static void _rx_handler(void * arg);
			static void _dre_handler(void * arg);
			static void _tx_handler(void * arg);
		};

	}
}

#endif