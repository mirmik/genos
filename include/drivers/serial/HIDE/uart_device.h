#ifndef GENOS_UART_DEVICE_H
#define GENOS_UART_DEVICE_H

#include <stdint.h>
#include <stdbool.h>

struct uart;

enum uart_parity_e {
	UART_PARITY_NONE,
	UART_PARITY_EVEN,
	UART_PARITY_ODD,
};

struct uart_params {
	uint32_t baud;
	uint8_t parity;
	int n_stop;
	int n_bits;
};

struct uart_ops {
	int (*putc)(struct uart *dev, int symbol);
	int (*getc)(struct uart *dev);
	
	int (*room)(struct uart *dev);
	int (*avail)(struct uart *dev);

	/*irq's control*/
	int (*txbegin)(struct uart* dev);
	int (*txend)(struct uart* dev);
	
	/*parametr's setup*/
	int (*setup)(struct uart *dev, const struct uart_params *params);
};

struct uart_driver_handlers {
	void (*tx)(void* drv);
	void (*rx)(void* drv, char c);
	void (*tx_complete)(void* drv);
};

struct uart {
	struct uart_ops *uops;
	
	/*driver ops*/
	struct uart_driver_handlers* drvhs;
	void* drvpriv;
};

__BEGIN_DECLS

static inline void uart_params_init(struct uart_params *params) {
	params -> baud = 9600;
	params -> parity = UART_PARITY_NONE;
	params -> n_stop = 1;
	params -> n_bits = 8;
};

__END_DECLS

#define uart_putc(u,c) 			((struct uart*)u)->uops->putc(((struct uart*)u),c)
#define uart_getc(u) 			((struct uart*)u)->uops->getc(((struct uart*)u))
#define uart_room(u) 			((struct uart*)u)->uops->room(((struct uart*)u))
#define uart_avail(u) 			((struct uart*)u)->uops->avail(((struct uart*)u))
#define uart_setup(u,params) 	((struct uart*)u)->uops->setup(((struct uart*)u),params)

//#define uart_hasrx(uart,c) ((struct uart*)uart)->uops->putc(((struct uart*)uart), c)
//#define uart_setup(uart,params) ((struct uart*)uart)->uops->setup(((struct uart*)uart),params)

#endif /* UART_DEVICE_H_ */
