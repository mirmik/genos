#ifndef GENOS_UART_DRIVER_BOTTOM_HALF_H
#define GENOS_UART_DRIVER_BOTTOM_HALF_H

//struct uart_drvbh_operations {
//	int(*write)(void* priv, const char* data, size_t size);
//	int(*set_rxnewchar_h)(void(*)(char));
//	int(*set_txempty_h)(void(*)());	
//};

struct uart_drvbh {
	struct ring_head 	txring;
	void* 				txbuffer;
}

#endif