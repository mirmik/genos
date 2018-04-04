#ifndef HAL_UART_H
#define HAL_UART_H

#include <gxx/event/delegate.h>

namespace genos {
	namespace hal {
		class uart {
		public:
			virtual void sendbyte(char b) = 0;
			virtual char recvbyte() = 0;
	
			virtual bool avail() = 0;
			virtual bool room() = 0;
	
			virtual void set_tx_irq_handler(gxx::fastaction act) = 0;
			virtual void set_tc_irq_handler(gxx::fastaction act) = 0;
			virtual void set_rx_irq_handler(gxx::fastaction act) = 0;
	
			virtual void enable_tx_irq(bool en) = 0;
			virtual void enable_tc_irq(bool en) = 0;
			virtual void enable_rx_irq(bool en) = 0;
	
	
			enum class parity {
				none,
				even,
				odd,
			};
	
			enum class stopbits {
				one,
				two,
			};
	
			enum class databits {
				eight,
				seven,
			};
		};
	}
}

#endif