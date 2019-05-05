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
	dlist_del(&pack->lnk);
	insend = pack;

	send_ptr = (char*) &pack->header;
	send_end = (char*) pack->endptr();

	uart_device_sendbyte(u, GSTUFF_START);
	send_crc = 0xFF;
	send_state = 0;

	uart_device_ctrirqs(u, UART_CTRIRQS_TXON);
}

void crow_uartgate::sended()
{
	DTRACE();
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
	DTRACE();
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
					uart_device_sendbyte(gate->u, GSTUFF_STUB);
					gate->send_state = 4;
					return;
				}

				gate->send_state = 2;
				uart_device_sendbyte(gate->u, gate->send_crc);
				return;
			}

			c = *gate->send_ptr++;
			strmcrc8(&gate->send_crc, c);

			if (c == GSTUFF_START || c == GSTUFF_STUB)
			{
				uart_device_sendbyte(gate->u, GSTUFF_STUB);
				gate->send_state = 1;
			}
			else
			{
				uart_device_sendbyte(gate->u, c);
			}

			return;

		case 1:
			switch (*(gate->send_ptr - 1))
			{
				case GSTUFF_START: uart_device_sendbyte(gate->u, GSTUFF_STUB_START); break;

				case GSTUFF_STUB: uart_device_sendbyte(gate->u, GSTUFF_STUB_STUB); break;
			}

			gate->send_state = 0;
			return;

		case 2:
			uart_device_sendbyte(gate->u, GSTUFF_START);
			gate->send_state = 3;
			return;

		case 3:
			uart_device_ctrirqs(gate->u, UART_CTRIRQS_TXOFF);
			gate->sended();
			return;

		case 4:
			switch ((char)gate->send_crc)
			{
				case GSTUFF_START: uart_device_sendbyte(gate->u, GSTUFF_STUB_START); break;

				case GSTUFF_STUB: uart_device_sendbyte(gate->u, GSTUFF_STUB_STUB); break;
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

	crow::travel(block);
}


void crow_uartgate::nblock_onestep()
{
	DTRACE();
	int sts;
	char c;

	if (ring_avail(&recvring))
	{

		c = ring_getc(&recvring, recvring_buffer);

		sts = gstuff_autorecv_newchar(&recver, c);

		if (sts == GSTUFF_NEWPACKAGE)
		{
			newline_handler();
		}
		else
		{
			//free(gate->rpack);
			//init_recv(gate);
		}
	}

	//dprln("exit from");
	//while(1);
}

void uartgate_rx_handler(void* arg)
{
	DTRACE();
	crow_uartgate* gate = (crow_uartgate*) arg;
	ring_putc(&gate->recvring, gate->recvring_buffer, uart_device_recvbyte(gate->u));
}


void uartgate_handler(void* arg, int variant)
{
	DTRACE();

	switch (variant)
	{
		case UART_IRQCODE_RX: uartgate_rx_handler(arg); break;

		case UART_IRQCODE_TX: uartgate_tx_handler(arg); break;

		case UART_IRQCODE_TC: BUG(); break;
	}
}

void crow_uartgate::init(struct uart_device * uart)
{
	DTRACE();
	u = uart;
	dlist_init(&to_send);
	insend = NULL;
	init_recv();

	ring_init(&recvring, PACKET_DATAADDR_SIZE_MAX);

	crow::link_gate(this, 0xF4);

	u->handarg = (void*) this;
	u->handler = uartgate_handler;

	uart_device_ctrirqs(u, UART_CTRIRQS_RXON);
}