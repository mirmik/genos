#ifndef GENOS_DRIVER_UARTRING_H
#define GENOS_DRIVER_UARTRING_H

#include <gxx/event/once_delegate.h>
#include <gxx/io/iostorage.h>
#include <gxx/bytering.h>
#include <hal/uart.h>

namespace drivers {

	class uartring_ostream : public gxx::io::ostream {
		hal::uart* uart;
		gxx::bytering ring;

	public:
		uartring_ostream(hal::uart* uart, gxx::buffer buf) : uart(uart), ring(buf) {}

		void init() {
			uart->set_tx_irq_handler(gxx::action(&uartring_ostream::tx_handler, this));
		}

	protected:
		int writeData(const char* dat, size_t sz) {
			if (uart->avail() && ring.empty()) {
				uart->sendbyte(*dat++);
				--sz;
			}

			while(sz) {
				int ret = ring.push(dat, sz);
				sz -= ret;
				dat += ret;
			}

			uart->enable_tx_irq(true);
		}

		void tx_handler() {
			uart->sendbyte(ring.pop());
			if (ring.empty()) uart->enable_tx_irq(false);
		}
	};


	class uartring_istorage : public gxx::io::istorage {
		hal::uart* uart;
		gxx::bytering ring;
		gxx::once_delegate_flag flag;

	public:
		uartring_istorage(hal::uart* uart, gxx::buffer buf) : uart(uart), ring(buf) {}

		void init() {
			uart->set_rx_irq_handler(gxx::action(&uartring_istorage::rx_handler, this));
			uart->enable_rx_irq(true);
		}

		int avail() { return ring.avail(); }
		void set_avail_callback(gxx::delegate<void> dlg) { flag.event(dlg); }
	protected:
		int readData(char* dat, size_t sz) {
			int ret = ring.popn(dat, sz);
			if (ring.empty()) flag.clr();
			return ret;
		}

		void rx_handler() {
			int c = uart->recvbyte();
			ring.push(c);
			flag.set();
		}
	};


	class uartring : public gxx::io::ostream, public gxx::io::istorage {
		hal::uart* uart;
		gxx::bytering rxring;
		gxx::bytering txring;
		gxx::once_delegate_flag flag;
	public:
		uartring(hal::uart* uart, gxx::buffer buf, gxx::buffer ibuf) : uart(uart), txring(buf), rxring(ibuf) {}
		uartring(hal::uart* uart, int len, int ilen) : uart(uart), txring(gxx::allocate_buffer(len)), rxring(gxx::allocate_buffer(ilen)) {}

		void init() {
			uart->set_tx_irq_handler(gxx::action(&uartring::tx_handler, this));
			uart->set_rx_irq_handler(gxx::action(&uartring::rx_handler, this));
			uart->enable_rx_irq(true);
		}

		int avail() { return rxring.avail(); }
		void set_avail_callback(gxx::delegate<void> dlg) { flag.event(dlg); }
	protected:
		int readData(char* dat, size_t sz) {
			int ret = rxring.popn(dat, sz);
			if (rxring.empty()) flag.clr();
			return ret;
		}

		int writeData(const char* dat, size_t sz) {
			if (uart->avail() && txring.empty()) {
				uart->sendbyte(*dat++);
				--sz;
			}

			while(sz) {
				int ret = txring.push(dat, sz);
				sz -= ret;
				dat += ret;
			}

			uart->enable_tx_irq(true);
		}

		void rx_handler() {
			int c = uart->recvbyte();
			rxring.push(c);
			flag.set();
		}

		void tx_handler() {
			uart->sendbyte(txring.pop());
			if (txring.empty()) uart->enable_tx_irq(false);
		}
	};
}

#endif