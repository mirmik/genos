#include <drivers/serial/avr/avr_serial_driver.h>

namespace genos {

	avr_serial_driver::avr_serial_driver(usart_regs* regs, int irqbase) {}
	avr_serial_driver::avr_serial_driver(const usart_data* data)
		: regs(data->regs), irqbase(data->irqbase) {}

	int avr_serial_driver::send_byte(char c) {
		regs->udr = c;
		return 1;
	}
	
	int avr_serial_driver::recv_byte() {
		return regs->udr;
	}
	
	int avr_serial_driver::can_recv() {
		return bits_mask(regs->ucsr_a, (1 << RXC0));
	}
	
	int avr_serial_driver::can_send() {
		return bits_mask(regs->ucsr_a, (1 << UDRE0));
	}
	
	int avr_serial_driver::enableRX(bool en) {
		bits_lvl(regs->ucsr_b, (1 << RXEN0), en);
	}
	
	int avr_serial_driver::enableTX(bool en) {
		bits_lvl(regs->ucsr_b, (1 << TXEN0), en);
	}
	
	int avr_serial_driver::irqEnableRX(bool en) {
		bits_lvl(regs->ucsr_b, (1 << RXCIE0), en);
	}
	
	int avr_serial_driver::irqEnableTX(bool en) {
		bits_lvl(regs->ucsr_b, (1 << UDRIE0), en);
	}
	
	int avr_serial_driver::irqEnableTC(bool en) {
		bits_lvl(regs->ucsr_b, (1 << TXCIE0), en);
	}
		
	int enable_transmitter(bool en) {
		irqEnableTX(en);
	}

	int avr_serial_driver::begin(
		int32_t baud, 
		Uart::Parity parity, 
		Uart::StopBits stopBits, 
		Uart::DataBits dataBits
	) {
		enableRX(true);
		enableTX(true);

		regs->ucsr_a |= 1 << U2X0;
		uint16_t baud_setting = (F_CPU / 4 / baud - 1) / 2;
	  
		regs->ubrr_h = baud_setting >> 8;
		regs->ubrr_l = baud_setting;
	
	// PARITY
	//	bits_mask_assign_bias(regs->ucsr_c, mode, 0b11, 4);
	
	// STOP BITS
	//	bits_mask_assign_bias(regs->ucsr_c, mode, 0b1, 3);
	
	// DATA BITS
	//	bits_mask_assign_bias(regs->ucsr_c, mode, 0b011, 1);
	//	bits_mask_assign(regs->ucsr_b, mode, 0b100);
	}

	int avr_serial_driver::end() {
		enableRX(false);
		enableTX(false);
	};
	
	void avr_serial_driver::set_handlers(
			irq_handler_t irq_handler_rx, void* arg_rx,
			irq_handler_t irq_handler_tx, void* arg_tx, 
			irq_handler_t irq_handler_tc, void* arg_tc
		) {
		set_irq_handler(irqbase + 0, irq_handler_rx, arg_rx);
		set_irq_handler(irqbase + 1, irq_handler_tx, arg_tx);
		set_irq_handler(irqbase + 2, irq_handler_tc, arg_tc);
	}
}