#ifndef GENOS_UART_DRIVER_BOTTOM_HALF_H
#define GENOS_UART_DRIVER_BOTTOM_HALF_H

struct uart_drvbh_operations {
	int(*write)(void* priv, const char* data, size_t size);
	int(*set_rxnewchar_h)(void(*)(char));
	int(*set_txempty_h)(void(*)());	
};

#endif