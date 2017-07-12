#include <dev/usart_chardev.h>
#include <genos/panic.h>

#include <gxx/util/bits.h>
#include <gxx/debug/dprint.h>

namespace periph {
	
	int Usart::putc(char c) {
		regs->udr = c;
		return 1;
	};
	
	int Usart::getc() {
		return regs->udr;
	};
	
	bool Usart::avail() {
		return bits_mask(regs->ucsr_a, 1 << 6);	
	};
	
	bool Usart::ready() {
		return bits_mask(regs->ucsr_a, 1 << 5);
	};
		
	void Usart::setRXHandler(Kernel::IRQHandler handler) {
		Kernel::IRQTable.setHandler(irqs->rx, handler);
	};
	
	void Usart::setTXHandler(Kernel::IRQHandler handler) {
		Kernel::IRQTable.setHandler(irqs->tx, handler);
	};

	void Usart::enableRXInt(bool b) {
		bits_lvl(regs->ucsr_b, 1 << 7, b);
	};
	
	void Usart::enableTXInt(bool b) {
		bits_lvl(regs->ucsr_b, 1 << 6, b);
	};
	
	void Usart::enableRX(bool b) {
		bits_lvl(regs->ucsr_b, 1 << 4, b);
	};
	
	void Usart::enableTX(bool b) {
		bits_lvl(regs->ucsr_b, 1 << 3, b);
	};

	void Usart::settings(const UartParams& params) {
		switch (params.parity) {
			case UART_PARITY_NONE: bits_mask_assign(regs->ucsr_c, 0b11 << 4, 0 << 4); break;
			case UART_PARITY_EVEN: bits_mask_assign(regs->ucsr_c, 0b11 << 4, 2 << 4); break;
			case UART_PARITY_ODD:  bits_mask_assign(regs->ucsr_c, 0b11 << 4, 3 << 4); break;
			default: panic("wrong");
		}

		switch (params.n_stop) {
			case 1: bits_mask_assign(regs->ucsr_c, 0b1 << 3, 0 << 3); break;
			case 2: bits_mask_assign(regs->ucsr_c, 0b1 << 3, 1 << 3); break;
			default: panic("wrong");
		}

		switch (params.n_bits) {
			case 5: 
				bits_mask_assign(regs->ucsr_c, 0b11 << 1, 0 << 1); 
				bits_mask_assign(regs->ucsr_b, 0b1  << 2, 0 << 1); 
				break;
			case 6: 
				bits_mask_assign(regs->ucsr_c, 0b11 << 1, 1 << 1);  
				bits_mask_assign(regs->ucsr_b, 0b1  << 2, 0 << 1);
				break;
			case 7: 
				bits_mask_assign(regs->ucsr_c, 0b11 << 1, 2 << 1);  
				bits_mask_assign(regs->ucsr_b, 0b1  << 2, 0 << 1);
				break;
			case 8: 
				bits_mask_assign(regs->ucsr_c, 0b11 << 1, 3 << 1);  
				bits_mask_assign(regs->ucsr_b, 0b1  << 2, 0 << 1);
				break;
			case 9: 
				bits_mask_assign(regs->ucsr_c, 0b11 << 1, 0 << 1);  
				bits_mask_assign(regs->ucsr_b, 0b1  << 2, 1 << 1);
				break;
			default: panic("wrong");
		}

		uint16_t baud_setting = (F_CPU / 4 / params.baud_rate - 1) / 2;

		if (((F_CPU == 16000000UL) && (params.baud_rate == 57600)) || (baud_setting >4095)) {
    		regs->ucsr_a &= ~(1 << U2X0);
  			baud_setting = (F_CPU / 8 / params.baud_rate - 1) / 2;
  		}

  		else {
			regs->ucsr_a |= 1 << U2X0;
  		}

  		regs->ubrr_h = baud_setting >> 8;
  		regs->ubrr_l = baud_setting;
	}

	Usart usart0(USART0, &USART0_IRQS);	
	Usart usart1(USART1, &USART1_IRQS);	

};