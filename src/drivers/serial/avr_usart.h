#ifndef GENOS_DRIVERS_AVR_UART
#define GENOS_DRIVERS_AVR_UART

#include <drivers/serial/uart.h>
#include <periph/regs/usart.h>

#include <hal/irqtable.h>

struct avr_usart : public uart {
	struct usart_regs * regs;
	uint8_t base_irq;

	int enable(bool en) override;
	int txirq(bool en) override;
	int recvbyte() override;
	int sendbyte(int symbol) override;
	int cantx() override;
	int hasrx() override;
	int setup(int32_t baud, enum uart_parity_e parity, uint8_t databits, uint8_t stopbits) override;

	int init(struct usart_regs * regs, int base_irq);
	
	avr_usart(){}
	avr_usart(struct usart_regs * regs, int base_irq) { init(regs, base_irq); }
};

__BEGIN_DECLS

__END_DECLS

#endif