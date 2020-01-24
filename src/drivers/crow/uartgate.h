#ifndef GENOS_DRIVERS_CROW_UARTGATE_H
#define GENOS_DRIVERS_CROW_UARTGATE_H

#include <crow/gateway.h>
#include <igris/protocols/gstuff/autorecv.h>
#include <igris/datastruct/ring.h>

#define PACKET_DATAADDR_SIZE_MAX 64
#define UARTGATE_RECVRING_SIZE (PACKET_DATAADDR_SIZE_MAX * 2)

void uartgate_rx_handler(void*);
void uartgate_tx_handler(void*);

class crow_uartgate : public crow::gateway 
{
private:
	struct uart_device * u;

	struct dlist_head to_send = DLIST_HEAD_INIT(to_send);
	struct crow::packet * volatile insend = nullptr;
	struct crow::packet * volatile rpack = nullptr;

	uint8_t send_state = 0;
	uint8_t send_crc = 0;
	char* send_ptr = nullptr;
	char* send_end = nullptr;		

	struct gstuff_autorecv recver;

	char recvring_buffer[UARTGATE_RECVRING_SIZE];
	struct ring_head recvring;

public:
	void nblock_onestep() override;
	void send(crow::packet* pack) override;
	void init(struct uart_device * uart, uint8_t addr);
	
private:
	void newline_handler(); 
	void init_recv();

	void do_send(crow::packet* pack);
	void sended();

	static void uartgate_handler(void* arg, int var);
	static void uartgate_rx_handler(void* arg);
	static void uartgate_tx_handler(void* arg);
};

#endif