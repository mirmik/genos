#ifndef GENOS_DRIVERS_AVR_UART
#define GENOS_DRIVERS_AVR_UART

#include <drivers/serial/uart.h>
#include <periph/regs/usart.h>

#include <hal/irqtable.h>

extern const struct uart_device_operations avr_usart_device_operations;

struct avr_usart_device : public uart_device
{	
	struct usart_regs * regs;
	uint8_t base_irq;

	int setup(int32_t baud, char parity, uint8_t databits, uint8_t stopbits) override;
	int enable(int en) override;
	int ctrirqs(uint8_t cmd) override;
	int recvbyte() override;
	int sendbyte(int c) override;
	int cantx() override;
	int hasrx() override;

	avr_usart_device(struct usart_regs * regs, int irqno) 
		: regs(regs), base_irq(irqno) 
	{}

private:
	int irqinit();
};

#define AVR_USART_DEVICE_DECLARE(name, regs, irqno)	\
struct avr_usart_device name = {regs, irqno}

#endif