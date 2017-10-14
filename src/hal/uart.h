#ifndef HAL_UART_H
#define HAL_UART_H

namespace hal {
	class uart {
	public:
		virtual void sendbyte(char b) = 0;
		virtual char recvbyte() = 0;

		virtual bool avail() = 0;
		virtual bool room() = 0;

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

#endif