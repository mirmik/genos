#ifndef GENOS_DRIVERS_CROW_UARTGATE_H
#define GENOS_DRIVERS_CROW_UARTGATE_H

#include <crow/gateway.h>
#include <igris/protocols/gstuff/autorecv.h>
#include <igris/datastruct/ring.h>

#define PACKET_DATAADDR_SIZE_MAX 64

void uartgate_rx_handler(void*);
void uartgate_tx_handler(void*);

struct crow_uartgate : public crow::gateway {
	struct uart_device * u;

	struct dlist_head to_send;
	struct crow::packet * insend;
	struct crow::packet * rpack;

	uint8_t send_state = 0;
	uint8_t send_crc;
	char* send_ptr;
	char* send_end;		

	struct gstuff_autorecv recver;

	char recvring_buffer[PACKET_DATAADDR_SIZE_MAX];
	struct ring_head recvring;

	void nblock_onestep() override;
	void send(crow::packet* pack) override;
	void init(struct uart_device * uart);
	
public:
	void newline_handler(); 
	void init_recv();

	void do_send(crow::packet* pack);
	void sended();
};

#endif