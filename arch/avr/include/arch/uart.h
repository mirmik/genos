#ifndef GENOS_AVR_UART_H
#define GENOS_AVR_UART_H

#include <avr/io.h>

#include <periph/usart.h>
#include <hal/uart.h>
#include <hal/irqtbl.h>

#include <gxx/util/bits.h>
#include <gxx/event/delegate.h>

namespace arch {

	class usart : public hal::uart {
		struct usart_regs* regs;
		int8_t irqbase;

	public:
		void sendbyte(char c) {
			regs->udr = c;
		}

		char recvbyte() {
			return regs->udr;
		}

		bool avail() {
			return regs->ucsr_a & (1 << RXC0);
		}

		bool room() {
			return  regs->ucsr_a & (1 << UDRE0);
		}

		void enable_rx(bool en = true) {
			bits_lvl(regs->ucsr_b, (1 << RXEN0), en);
		}

		void enable_tx(bool en = true) {
			bits_lvl(regs->ucsr_b, (1 << TXEN0), en);
		}

		void enable(bool en = true) {
			enable_rx(en);
			enable_tx(en);
		}

		void enable_rx_irq(bool en) override {
			bits_lvl(regs->ucsr_b, (1 << RXCIE0), en);
		}

		void enable_tx_irq(bool en) override {
			bits_lvl(regs->ucsr_b, (1 << UDRIE0), en);
		}

		void enable_tc_irq(bool en) override {
			bits_lvl(regs->ucsr_b, (1 << TXCIE0), en);
		}

		void setup(
			int32_t baud = 115200, 
			hal::uart::parity parity = hal::uart::parity::none, 
			hal::uart::stopbits stopbits = hal::uart::stopbits::one, 
			hal::uart::databits databits = hal::uart::databits::eight
		) 
		{	
			regs->ucsr_a |= 1 << U2X0;
			uint16_t baud_setting = (F_CPU / 4 / baud - 1) / 2;
			 
			regs->ubrr_h = baud_setting >> 8;
			regs->ubrr_l = baud_setting;

			// PARITY
			uint8_t sparity = 0;
			bits_mask_assign_bias(regs->ucsr_c, sparity, 0b11, 4);
			
			// STOP BITS
			uint8_t sstopbits = stopbits == hal::uart::stopbits::one ? 0 : 1;
			bits_mask_assign_bias(regs->ucsr_c, sstopbits, 0b1, 3);
			
			// DATA BITS
			//uint8_t sbits = stopbits == hal::uart::stopbits::one ? 0 : 1;
			//bits_mask_assign_bias(regs->ucsr_c, mode, 0b011, 1);
			//bits_mask_assign(regs->ucsr_b, mode, 0b100);
		}

		usart(struct usart_data data) : regs(data.regs), irqbase(data.irqbase) {}
	
		void set_rx_irq_handler(gxx::fastaction handler) override {
			hal::irqtbl::set_handler(irqbase + 0, handler);
		}

		void set_tx_irq_handler(gxx::fastaction handler) override {
			hal::irqtbl::set_handler(irqbase + 1, handler);
		}

		void set_tc_irq_handler(gxx::fastaction handler) override {
			hal::irqtbl::set_handler(irqbase + 2, handler);
		}
	};
}

#endif
