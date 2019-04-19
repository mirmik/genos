#ifndef UART_DEVICE_H_
#define UART_DEVICE_H_

#include <sys/cdefs.h>
#include <defs/uartdefs.h>
#include <stdbool.h>

#define UART_IRQCODE_RX 0
#define UART_IRQCODE_TX 1 // Можно положить следующий байт
#define UART_IRQCODE_TC 2 // Конвеер завершил свою работу.

namespace genos
{
	namespace drivers
	{
		using uart_handler_t = void(*)(void*, int);

		/**
			Абстракция над реализациями приёмо-передатчика потокового
			ввода - вывода.
		*/
		class uart
		{
		public:
			virtual int enable(bool en = true) = 0;
			virtual int txirq(bool en) = 0;
			virtual int recvbyte() = 0;
			virtual int sendbyte(int symbol) = 0;
			virtual int cantx() = 0;
			virtual int hasrx() = 0;

			virtual int setup(int32_t baud     = 9600,
			                  char parity      = 'n',
			                  uint8_t databits = 8,
			                  uint8_t stopbits = 1
			                 ) = 0;

			void set_handler(uart_handler_t handler, void * handarg)
			{
				this->handler = handler;
				this->handarg = handarg;
			}

		private:
			uart_handler_t  handler;
			void *          handarg;
		};
	}
}

#endif
