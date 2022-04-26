#ifndef GENOS_DRIVERS_CROW_UARTGATE_H
#define GENOS_DRIVERS_CROW_UARTGATE_H

#include <drivers/serial/uart.h>
#include <crow/gateway.h>
#include <igris/protocols/gstuff.h>
#include <igris/datastruct/ring.h>

#define PACKET_DATAADDR_SIZE_MAX 64
#define UARTGATE_RECVRING_SIZE (PACKET_DATAADDR_SIZE_MAX * 2)

void uartgate_rx_handler(void*);
void uartgate_tx_handler(void*);

class crow_uartgate : public crow::gateway 
{
private:
	genos::uart * u;

	struct dlist_head to_send = DLIST_HEAD_INIT(to_send);
	struct crow::packet * volatile insend = nullptr;
	struct crow::packet * volatile rpack = nullptr;

	volatile uint8_t send_state = 0;
	uint8_t send_crc = 0;
	char* send_ptr = nullptr;
	char* send_end = nullptr;		

	struct gstuff_autorecv recver;

	char recvring_buffer[UARTGATE_RECVRING_SIZE];
	struct ring_head recvring;

	uint16_t usart_error_counter = 0;

public:
	void nblock_onestep() override;
	void send(crow::packet* pack) override;
	void finish() override {}
	void init(genos::uart * uart, uint8_t addr);
	
private:
	void newline_handler(); 
	void error_handler(); 
	void init_recv();

	void do_send(crow::packet* pack);
	void sended();

	static void uartgate_handler(void* arg, int var);
	static void uartgate_rx_handler(void* arg);
	static void uartgate_tx_handler(void* arg);
	static void uartgate_error_handler(void* arg, int var);
};

#endif