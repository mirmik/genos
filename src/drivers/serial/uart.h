#ifndef UART_DEVICE_H_
#define UART_DEVICE_H_

#include <sys/cdefs.h>
#include <defs/uartdefs.h>
#include <igris/util/retype.h>

#include <stdint.h>
#include <stdbool.h>

#define UART_IRQCODE_RX 0
#define UART_IRQCODE_TX 1 // Можно положить следующий байт
#define UART_IRQCODE_TC 2 // Конвеер завершил свою работу.

#define UART_CTRIRQS_RXON 0
#define UART_CTRIRQS_RXOFF 1
#define UART_CTRIRQS_TXON 2
#define UART_CTRIRQS_TXOFF 3
#define UART_CTRIRQS_TCON 4
#define UART_CTRIRQS_TCOFF 5

/*#ifdef __cplusplus
namespace genos
{
	namespace drivers
	{
		using uart_handler_t = void(*)(void*, int);

		/**
			Абстракция над реализациями приёмо-передатчика потокового
			ввода - вывода.
		*/
/*		class uart
		{
		public:
			virtual int enable(bool en = true) = 0;
			virtual int ctrirqs(uint8_t cmd) = 0;
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

		protected:
			uart_handler_t  handler;
			void *          handarg;
		};
	}
}
#endif
*/

struct uart_device;
typedef void(*uart_irq_handler_t)(void*, int);

typedef int(*uart_enable_t)(struct uart_device * u, int);
typedef int(*uart_ctrirqs_t)(struct uart_device * u, uint8_t);
typedef int(*uart_recvbyte_t)(struct uart_device * u);
typedef int(*uart_sendbyte_t)(struct uart_device * u, int);
typedef int(*uart_cantx_t)(struct uart_device * u);
typedef int(*uart_hasrx_t)(struct uart_device * u);
typedef int(*uart_setup_t)(struct uart_device * u,
                           int32_t, char, uint8_t, uint8_t);

struct uart_device_operations
{
	uart_enable_t enable;
	uart_ctrirqs_t ctrirqs;
	uart_recvbyte_t recvbyte;
	uart_sendbyte_t sendbyte;
	uart_cantx_t cantx;
	uart_hasrx_t hasrx;
	uart_setup_t setup;
};

struct uart_device
{
	const struct uart_device_operations * u_ops;

	uart_irq_handler_t  handler;
	void*               handarg;
};

__BEGIN_DECLS

static inline void
uart_device_init(struct uart_device * u,
                 const struct uart_device_operations * u_ops)
{
	u -> u_ops = u_ops;
}

static inline int
uart_device_setup(void* dev, int32_t baud, char parity,
                  uint8_t databits, uint8_t stopbits) 
{
	RETYPE(struct uart_device*, u, dev);
	return u->u_ops->setup(u, baud, parity, databits, stopbits);
}

static inline 
int uart_device_sendbyte(void* dev, int symbol) 
{
	RETYPE(struct uart_device*, u, dev);
	return u->u_ops->sendbyte(u, symbol);		
}

static inline 
int uart_device_recvbyte(void* dev) 
{
	RETYPE(struct uart_device*, u, dev);
	return u->u_ops->recvbyte(u);		
}

static inline 
int uart_device_cantx(void* dev) 
{
	RETYPE(struct uart_device*, u, dev);
	return u->u_ops->cantx(u);		
}

static inline 
int uart_device_hasrx(void* dev) 
{
	RETYPE(struct uart_device*, u, dev);
	return u->u_ops->hasrx(u);		
}

static inline 
int uart_device_ctrirqs(struct uart_device* u, uint8_t cmd) 
{
	return u->u_ops->ctrirqs(u, cmd);		
}

__END_DECLS

#endif
