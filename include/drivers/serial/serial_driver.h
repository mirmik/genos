#ifndef GENOS_SERIAL_DRIVER_H
#define GENOS_SERIAL_DRIVER_H

#include <drivers/serial/uart_params.h>
#include <kernel/irq.h>

namespace genos {

	class serial_driver {
	public:
		virtual int putc(char c) = 0;
		virtual int getc() = 0;

		virtual int avail() = 0;
		virtual int room() = 0;

		virtual int transmit_enable(bool en);
		//virtual int enable_receiver(bool en);

		virtual int begin(int32_t baud = 9600, 
			Uart::Parity parity = Uart::ParityNone, 
			Uart::StopBits stopBits = Uart::StopBitsOne, 
			Uart::DataBits dataBits = Uart::DataBitsEight
		) = 0;

		virtual int end() = 0;

		virtual void set_handlers(
			irq_handler_t irq_handler_rx, void* arg_rx,
			irq_handler_t irq_handler_tx, void* arg_tx, 
			irq_handler_t irq_handler_tc, void* arg_tc
		) = 0;
	};

}

#endif