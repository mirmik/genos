#ifndef GENOS_UARTRING_H
#define GENOS_UARTRING_H

#include <genos/fs/cdev.h>
#include <genos/hal/uart.h>
#include <genos/hal/atomic.h>
#include <gxx/bytering.h>

namespace genos {

	struct uartring : public genos::cstream {
		genos::hal::uart* uart;
		
		gxx::bytering rxring;
		gxx::bytering txring;
	public:
		uartring(genos::hal::uart* uart, gxx::buffer buf, gxx::buffer ibuf) 
			: uart(uart), txring(buf), rxring(ibuf) {}

		uartring(genos::hal::uart* uart, int len, int ilen) 
			: uart(uart), txring(gxx::allocate_buffer(len)), rxring(gxx::allocate_buffer(ilen)) {}

		void init() {
			uart->set_tx_irq_handler(gxx::fastaction(&uartring::tx_handler, this));
			uart->set_rx_irq_handler(gxx::fastaction(&uartring::rx_handler, this));
			uart->enable_rx_irq(true);
		}

		int avail() override { 
			return rxring.avail(); 
		}
		
		int room() override { 
			return txring.avail(); 
		}

		int read(char* dat, size_t sz, size_t* off) override {
			genos::atomic_section_enter();
			int ret = rxring.popn(dat, sz);
			if (rxring.empty()) avail_flag.clr();
			genos::atomic_section_leave();
			return ret;
		}

		int write(const char* dat, size_t sz, size_t off) override {
			int ret = 0;

			genos::atomic_section_enter();
			if (uart->room() && txring.empty()) {
				uart->sendbyte(*dat++);
				--sz;
				empty_flag.clr();
				uart->enable_tx_irq(true);
				++ret;
			}

			ret += txring.push(dat, sz);
			genos::atomic_section_leave();

			return ret;
		}

	protected:
		void rx_handler() {
			int c = uart->recvbyte();
			rxring.push(c);
			avail_flag.set();
		}

		void tx_handler() {
			uart->sendbyte(txring.pop());
			if (txring.empty()) {
				uart->enable_tx_irq(false);
				empty_flag.set();
			}
		}
	};

}

#endif