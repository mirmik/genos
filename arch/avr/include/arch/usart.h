#ifndef GENOS_AVR_UART_H
#define GENOS_AVR_UART_H

#include <periph/regs/usart.h>
#include <genos/hal/uart.h>
#include <genos/hal/irqtbl.h>

#include <avr/io.h>

#include <gxx/util/bits.h>
#include <gxx/event/delegate.h>

namespace arch {
	class usart : public genos::hal::uart {
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
			genos::hal::uart::parity 	parity 		= genos::hal::uart::parity::none, 
			genos::hal::uart::stopbits 	stopbits 	= genos::hal::uart::stopbits::one, 
			genos::hal::uart::databits 	databits 	= genos::hal::uart::databits::eight
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
			uint8_t sstopbits = stopbits ==genos::hal::uart::stopbits::one ? 0 : 1;
			bits_mask_assign_bias(regs->ucsr_c, sstopbits, 0b1, 3);
			
			// DATA BITS
			//uint8_t sbits = stopbits ==genos::hal::uart::stopbits::one ? 0 : 1;
			//bits_mask_assign_bias(regs->ucsr_c, mode, 0b011, 1);
			//bits_mask_assign(regs->ucsr_b, mode, 0b100);
		}

		usart(struct usart_regs* regs, int8_t irqbase) : regs(regs), irqbase(irqbase) {}
	
		void set_rx_irq_handler(gxx::fastaction handler) override {
			genos::hal::irqtbl::set_handler(irqbase + 0, handler);
		}

		void set_tx_irq_handler(gxx::fastaction handler) override {
			genos::hal::irqtbl::set_handler(irqbase + 1, handler);
		}

		void set_tc_irq_handler(gxx::fastaction handler) override {
			genos::hal::irqtbl::set_handler(irqbase + 2, handler);
		}
	};

/*#if defined (CHIP_ATMEGA2560)
extern usart usart0(USART0, ATMEGA_IRQ_U0RX);
extern usart usart1(USART1, ATMEGA_IRQ_U1RX);
extern usart usart2(USART2, ATMEGA_IRQ_U2RX);
#elif defined (CHIP_ATMEGA328P)
extern usart usart0(USART0, ATMEGA_IRQ_U0RX);
#else 
#	error "unrecognized chip"
#endif*/
}

#endif
