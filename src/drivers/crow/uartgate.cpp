#define NOTRACE 1

#include <drivers/crow/uartgate.h>
#include <crow/tower.h>

#include <igris/sync/syslock.h>
#include <gxx/panic.h>

#include <gxx/gstuff/gstuff.h>

#include <drivers/serial/uart.h>
#include <hal/irq.h>

#include <gxx/util/crc.h>
#include <gxx/trace.h>

#include <hal/board.h>

void crow_uartgate::do_send(crow::packet* pack)
{
	TRACE();
	dlist_del(&pack->lnk);
	insend = pack;

	send_ptr = (char*) &pack->header;
	send_end = (char*) pack->endptr();

	u->sendbyte(GSTUFF_START);
	send_crc = 0xFF;
	send_state = 0;

	u->txirq(true);
}

void crow_uartgate::sended()
{
	TRACE();
	auto tmp = insend;

	if (dlist_empty(&to_send))
	{
		insend = nullptr;
	}
	else
	{
		struct crow::packet * entry = dlist_first_entry(&to_send, struct crow::packet, lnk);
		do_send(entry);
	}

	crow::return_to_tower(tmp, CROW_SENDED);
}

void crow_uartgate::send(crow::packet* pack)
{
	TRACE();
	system_lock();

	if (insend == nullptr && dlist_empty(&to_send))
	{
		do_send(pack);
	}
	else
	{
		dlist_move_tail(&pack->lnk, &to_send);
	}

	system_unlock();
}

void uartgate_tx_handler(void* arg)
{
	TRACE();
	crow_uartgate* gate = (crow_uartgate*) arg;
	char c;

	switch (gate->send_state)
	{
		case 0:
			if (gate->send_ptr == gate->send_end)
			{

				if ((char)gate->send_crc == GSTUFF_START || (char)gate->send_crc == GSTUFF_STUB)
				{
					gate->u->sendbyte(GSTUFF_STUB);
					gate->send_state = 4;
					return;
				}

				gate->send_state = 2;
				gate->u->sendbyte(gate->send_crc);
				return;
			}

			c = *gate->send_ptr++;
			strmcrc8(&gate->send_crc, c);

			if (c == GSTUFF_START || c == GSTUFF_STUB)
			{
				gate->u->sendbyte(GSTUFF_STUB);
				gate->send_state = 1;
			}
			else
			{
				gate->u->sendbyte(c);
			}

			return;

		case 1:
			switch (*(gate->send_ptr - 1))
			{
				case GSTUFF_START: gate->u->sendbyte(GSTUFF_STUB_START); break;
				case GSTUFF_STUB: gate->u->sendbyte(GSTUFF_STUB_STUB); break;
			}

			gate->send_state = 0;
			return;

		case 2:
			gate->u->sendbyte(GSTUFF_START);
			gate->send_state = 3;
			return;

		case 3:
			gate->u->txirq(false);
			gate->sended();
			return;

		case 4:
			switch ((char)gate->send_crc)
			{
				case GSTUFF_START: gate->u->sendbyte(GSTUFF_STUB_START); break;
				case GSTUFF_STUB: gate->u->sendbyte(GSTUFF_STUB_STUB); break;
			}

			gate->send_state = 2;
			return;
	}
}

void crow_uartgate::init_recv()
{
	TRACE();
	system_lock();
	rpack = (struct crow::packet*) crow::allocate_packet(PACKET_DATAADDR_SIZE_MAX);
	
	gstuff_autorecv_setbuf(&recver, (char*)&rpack->header, PACKET_DATAADDR_SIZE_MAX);
	gstuff_autorecv_reset(&recver);

	system_unlock();
}

void crow_uartgate::newline_handler() 
{
	TRACE();
	crow::packet* block = rpack;
	
	init_recv();

	block->revert_gate(id);
	crow::packet_initialization(block, this);

	crow::travel(block);
}


void crow_uartgate::nblock_onestep()
{
	TRACE();
	int sts;
	char c;

	if (ring_avail(&recvring))
	{

		c = ring_getc(&recvring, recvring_buffer);

		sts = gstuff_autorecv_newchar(&recver, c);

		if (sts == GSTUFF_NEWPACKAGE) {
			newline_handler();
		} else {
		//free(gate->rpack);
		//init_recv(gate);
		}
	}

	//dprln("exit from");
	//while(1);
}

void uartgate_rx_handler(void* arg)
{
	TRACE();
	crow_uartgate* gate = (crow_uartgate*) arg;
	ring_putc(&gate->recvring, gate->recvring_buffer, gate->u->recvbyte());
}


void uartgate_handler(void* arg, int variant)
{
	TRACE();
	switch(variant) {
		case UART_IRQCODE_RX: uartgate_rx_handler(arg); break;
		case UART_IRQCODE_TX: uartgate_tx_handler(arg); break;
		case UART_IRQCODE_TC: PANIC_TRACED(); break;
	}
}

void crow_uartgate::init(struct uart * uart)
{
	TRACE();
	u = uart;
	dlist_init(&to_send);
	insend = NULL;
	init_recv();

	ring_init(&recvring, PACKET_DATAADDR_SIZE_MAX);

	crow::link_gate(this, 0xF4);

	u->handarg = (void*) this;
	u->handler = uartgate_handler;

	u->enable(true);
}