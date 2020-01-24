#define NODTRACE 1
#include <igris/dtrace.h>

#include <drivers/serial/uart.h>
#include <drivers/crow/uartgate.h>
#include <crow/tower.h>

#include <igris/sync/syslock.h>
#include <igris/util/bug.h>
#include <drivers/serial/uart.h>

#include <hal/irq.h>
#include <hal/board.h>

void crow_uartgate::do_send(crow::packet* pack)
{
	DTRACE();
	assert(pack);

	system_lock();

	dlist_del_init(&pack->lnk);
	insend = pack;

	send_ptr = (char*) &pack->header;
	send_end = (char*) pack->endptr();

	u->sendbyte(GSTUFF_START);
	send_crc = 0xFF;
	send_state = 0;

	u->ctrirqs(UART_CTRIRQS_TXON);

	system_unlock();
}

void crow_uartgate::sended()
{
	system_lock();
	DTRACE();
	assert(insend);

	crow::packet * tmp = insend;
	assert(tmp);

	if (dlist_empty(&to_send))
	{
		insend = nullptr;
	}
	else
	{
		struct crow::packet * entry = dlist_first_entry(&to_send, struct crow::packet, lnk);
		do_send(entry);
	}

	system_unlock();
	crow::return_to_tower(tmp, CROW_SENDED);
}

void crow_uartgate::send(crow::packet* pack)
{
	DTRACE();
	system_lock();

	assert(pack);

	//dprptrln(insend);
	//dprln(dlist_empty(&to_send));

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

void crow_uartgate::uartgate_tx_handler(void* arg)
{
	DTRACE();
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
			gate->u->ctrirqs(UART_CTRIRQS_TXOFF);
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
	DTRACE();
	system_lock();
	rpack = (struct crow::packet*) crow::allocate_packet(PACKET_DATAADDR_SIZE_MAX);
	if (rpack == nullptr) 
	{
		return;
		system_unlock();
	}

	gstuff_autorecv_setbuf(&recver, (char*)&rpack->header, PACKET_DATAADDR_SIZE_MAX);
	gstuff_autorecv_reset(&recver);

	system_unlock();
}

void crow_uartgate::newline_handler()
{
	DTRACE();
	crow::packet* block = rpack;

	init_recv();

	block->revert_gate(id);
	crow::packet_initialization(block, this);

	crow::nocontrol_travel(block);
}


void crow_uartgate::nblock_onestep()
{
	//DTRACE();
	int sts;
	char c;

	if (rpack==nullptr)
		init_recv();

	while (1)
	{
		system_lock();
		int ravail = ring_avail(&recvring);
		system_unlock();
		if (ravail == 0) break;
		
		if (rpack == nullptr)
			return;

		system_lock();
		c = ring_getc(&recvring, recvring_buffer);
		system_unlock();

		sts = gstuff_autorecv_newchar(&recver, c);

		if (sts == GSTUFF_NEWPACKAGE)
		{
			newline_handler();
		}
		else if (sts == GSTUFF_CONTINUE)
		{
			return;
		}
		else 
		{
			// RESET
		}
	}
}

void crow_uartgate::uartgate_rx_handler(void* arg)
{
	DTRACE();
	crow_uartgate* gate = (crow_uartgate*) arg;
	ring_putc(&gate->recvring, gate->recvring_buffer, gate->u->recvbyte());
}


void crow_uartgate::uartgate_handler(void* arg, int variant)
{
	DTRACE();

	switch (variant)
	{
		case UART_IRQCODE_RX: uartgate_rx_handler(arg); break;

		case UART_IRQCODE_TX: uartgate_tx_handler(arg); break;

		case UART_IRQCODE_TC: BUG(); break;
	}
}

void crow_uartgate::init(struct uart_device * uart, uint8_t addr)
{
	DTRACE();
	u = uart;
	dlist_init(&to_send);
	//insend = NULL;
	init_recv();

	ring_init(&recvring, UARTGATE_RECVRING_SIZE);

	crow::link_gate(this, addr);

	u->handarg = (void*) this;
	u->handler = uartgate_handler;

    u -> ctrirqs(UART_CTRIRQS_RXON);
}