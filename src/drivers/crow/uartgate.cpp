#include <drivers/crow/uartgate.h>
#include <crow/tower.h>

#include <gxx/syslock.h>
#include <gxx/panic.h>

#include <gxx/gstuff/gstuff.h>

#include <drivers/serial/uart.h>

#include <gxx/util/crc.h>

static void crow_uartgate_do_send(struct crow_uartgate * ugate, crowket* pack)
{
	dlist_del(&pack->lnk);
	ugate->insend = pack;

	ugate->send_ptr = (char*) &pack->header;
	ugate->send_end = (char*) pack->endptr();

	ugate->u->sendbyte(GSTUFF_START);
	ugate->send_crc = 0xFF;
	ugate->send_state = 0;

	ugate->u->txirq(true);
}

void crow_uartgate_sended(struct crow_uartgate * ugate)
{
	auto tmp = ugate->insend;

	if (dlist_empty(&ugate->to_send))
	{
		ugate->insend = nullptr;
	}
	else
	{
		struct crowket * entry = dlist_first_entry(&ugate->to_send, struct crowket, lnk);
		crow_uartgate_do_send(ugate, entry);
	}

	crow_return_to_tower(tmp, CROW_SENDED);
}

void crow_uartgate_nblock_onestep(struct crow_gw * g)
{}

void crow_uartgate_send(struct crow_gw * g, crowket* pack)
{
	crow_uartgate* ugate = mcast_out(g, crow_uartgate, gw);

	system_lock();
	bool b = ugate->insend == nullptr && dlist_empty(&ugate->to_send);

	if (b)
	{
		crow_uartgate_do_send(ugate, pack);
	}
	else
	{
		dlist_move_tail(&pack->lnk, &ugate->to_send);
	}

	system_unlock();
}

void uartgate_tx_handler(void* arg)
{
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
			crow_uartgate_sended(gate);
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

void uartgate_rx_handler(void* arg)
{
	crow_uartgate* gate = (crow_uartgate*) arg;
	char c = gate->u->recvbyte();
	//gate->recver.newchar(c);
}

void uartgate_handler(void* arg, int variant)
{
	switch(variant) {
		case UART_IRQCODE_RX: uartgate_rx_handler(arg); break;
		case UART_IRQCODE_TX: uartgate_tx_handler(arg); break;
		case UART_IRQCODE_TC: PANIC_TRACED(); break;
	}
}

static inline void init_recv(struct crow_uartgate * ugate)
{
	system_lock();
	ugate->rpack = (struct crowket*) malloc(PACKET_DATAADDR_SIZE_MAX + sizeof(crowket) - sizeof(crow_header));
	//recver.init(gxx::buffer((char*)&rpack->header, PACKET_DATAADDR_SIZE_MAX));
	system_unlock();
}

void crow_uartgate_init(struct crow_uartgate * ugate, struct uart * uart)
{
	ugate->u = uart;
	dlist_init(&ugate->to_send);
	ugate->insend = NULL;

	ugate->gw.ops = &crow_uartgate_ops;
	crow_link_gate(&ugate->gw, 10);

	//u->set_rx_irq_handler(uartgate_rx_handler, this);
	//u->set_tx_irq_handler(uartgate_tx_handler, this);
	ugate->u->handarg = (void*) ugate;
	ugate->u->handler = uartgate_handler;

	ugate->u->enable(true);

	//u->enable_rx_irq(true);
	//recver.set_callback(gxx::make_delegate(&uartgate::handler, this));
	init_recv(ugate);
}