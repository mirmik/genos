#ifndef GENOS_DRIVER_UARTRING_H
#define GENOS_DRIVER_UARTRING_H

#include <gxx/event/flag.h>
#include <gxx/io/iostorage.h>
#include <gxx/bytering.h>
#include <hal/uart.h>

namespace drivers {
	class uartring : public gxx::io::ostream, public gxx::io::istorage {
		genos::hal::uart* uart;
		gxx::bytering rxring;
		gxx::bytering txring;
	public:
		gxx::event::action_flag rx_avail_flag;
		gxx::event::action_flag tx_empty_flag;

		uartring(genos::hal::uart* uart, gxx::buffer buf, gxx::buffer ibuf) : uart(uart), txring(buf), rxring(ibuf) {}
		uartring(genos::hal::uart* uart, int len, int ilen) : uart(uart), txring(gxx::allocate_buffer(len)), rxring(gxx::allocate_buffer(ilen)) {}

		void init() {
			uart->set_tx_irq_handler(gxx::fastaction(&uartring::tx_handler, this));
			uart->set_rx_irq_handler(gxx::fastaction(&uartring::rx_handler, this));
			uart->enable_rx_irq(true);
		}

		int avail() { return rxring.avail(); }
		//void set_avail_flag(gxx::event::flag& flg) { flag = &flg; }
	protected:
		int readData(char* dat, size_t sz) {
			//dprln(rxring.avail());
			system_lock();
			int ret = rxring.popn(dat, sz);
			if (rxring.empty()) rx_avail_flag.clr();
			system_unlock();
			return ret;
		}

		int writeData(const char* dat, size_t sz) {
			system_lock();
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
			system_unlock();
		}

		void rx_handler() {
			int c = uart->recvbyte();
			rxring.push(c);
			rx_avail_flag.set();
		}

		void tx_handler() {
			uart->sendbyte(txring.pop());
			if (txring.empty()) uart->enable_tx_irq(false);
		}
	};
}

#endif