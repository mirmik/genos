#ifndef GENOS_DRIVERS_AVR_UART
#define GENOS_DRIVERS_AVR_UART

#include <drivers/serial/uart.h>
#include <periph/regs/usart.h>

#include <hal/irqtable.h>

struct avr_usart : public uart {
	struct usart_regs * regs;
	uint8_t base_irq;

	int enable(bool en=true) override;
	int txirq(bool en) override;
	int recvbyte() override;
	int sendbyte(int symbol) override;
	int cantx() override;
	int hasrx() override;
	int setup(int32_t baud, enum uart_parity_e parity=UART_PARITY_NONE, uint8_t databits=8, uint8_t stopbits=1) override;
	
	void init(struct usart_regs * regs, int base_irq) { this->regs=regs; this->base_irq=base_irq; }

	avr_usart(struct usart_regs * regs, int base_irq) : regs(regs), base_irq(base_irq) { }
	avr_usart(){}
private: 
	int irqinit();
};

__BEGIN_DECLS

__END_DECLS

#endif