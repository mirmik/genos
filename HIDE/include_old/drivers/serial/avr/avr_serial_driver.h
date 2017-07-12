#ifndef AVR_SERIAL_DRIVER_H
#define AVR_SERIAL_DRIVER_H

#include <asm/usart.h>
#include <drivers/serial/serial_driver.h>

namespace genos {

	class avr_serial_driver : public serial_driver {
	public:
		usart_regs* regs;
		uint8_t 	irqbase;
	
	public:
		avr_serial_driver(usart_regs* regs, int irqbase);
		avr_serial_driver(const usart_data* data);
	
	public:
	
	//serial_driver interface
		int send_byte(char c) override;
		int recv_byte() override;

		int can_send() override;
		int can_recv() override;

		int begin(int32_t baud = 9600, 
			Uart::Parity parity = Uart::ParityNone, 
			Uart::StopBits stopBits = Uart::StopBitsOne, 
			Uart::DataBits dataBits = Uart::DataBitsEight
		) override;

		int end() override;

		void set_handlers(
			irq_handler_t irq_handler_rx, void* arg_rx,
			irq_handler_t irq_handler_tx, void* arg_tx, 
			irq_handler_t irq_handler_tc, void* arg_tc
		) override;
	
	//control functions
		int enable(bool en);
		int enableRX(bool en);
		int enableTX(bool en);
	
		int irqEnableRX(bool en);
		int irqEnableTX(bool en);
		int irqEnableTC(bool en);
	};

}

#endif