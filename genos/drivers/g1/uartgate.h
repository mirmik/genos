#ifndef GENOS_DRIVER_G1_UARTGATE_H
#define GENOS_DRIVER_G1_UARTGATE_H

#include <genos/hal/uart.h>
#include <g1/gateway.h>
#include <gxx/gstuff/automate.h>
#include <gxx/syslock.h>

namespace genos {
	struct uartgate : public g1::gateway {
		gxx::dlist<g1::packet, &g1::packet::lnk> to_send;
		g1::packet* insend;

		genos::hal::uart* u;

		//gxx::gstuff::automate reader;
		size_t maxpacksz;

		uint8_t send_state = 0;
		uint8_t send_crc;
		char* send_ptr;
		char* send_end;		

		uartgate(genos::hal::uart* u, size_t maxpacksz) : u(u) {}

		g1::packet* rpack = nullptr;
		gxx::gstuff::automate recver;
		//uint16_t len;

		void init() {
			u->set_rx_irq_handler(gxx::fastaction(&genos::uartgate::rx_handler, this));
			u->set_tx_irq_handler(gxx::fastaction(&genos::uartgate::tx_handler, this));
			u->enable_rx_irq(true);
			recver.set_callback(gxx::make_delegate(&uartgate::handler, this));
			init_recv();			
		}

		void init_recv() {
			gxx::system_lock();
			rpack = (g1::packet*) malloc(128 + sizeof(g1::packet) - sizeof(g1::packet_header));
			recver.init(gxx::buffer((char*)&rpack->header, 128));
			gxx::system_unlock();
		}

		void do_send(g1::packet* pack) {
			gxx::dlist<g1::packet, &g1::packet::lnk>::unbind(*pack);
			insend = pack;

			send_ptr = (char*) &pack->header;
			send_end = (char*) pack->endptr();
			
			u->sendbyte(gxx::gmsg::strt);
			send_crc = 0xFF;
			send_state = 0;

			u->enable_tx_irq(true);
		}

		void send(g1::packet* pack) override {
			gxx::system_lock();
			bool b = insend == nullptr && to_send.empty();
			if (b) {
				do_send(pack);
			}
			else {
				to_send.move_back(*pack);
			}
			gxx::system_unlock();
		}

		void rx_handler() {
			char c = u->recvbyte();
			recver.newchar(c);
			//debug_putchar(c);
		}

		void handler(gxx::buffer) {
			gxx::system_lock();
			g1::packet* block = (g1::packet*)realloc(rpack, rpack->header.flen + sizeof(g1::packet) - sizeof(g1::packet_header));
			gxx::system_unlock();
			init_recv();

			block->revert_stage(id);

			g1::packet_initialization(block, this);
			g1::travel(block);
		}

		void tx_handler() {
			char c;
			switch (send_state) {
				case 0:
					if (send_ptr == send_end) {
						
						if ((char)send_crc == gxx::gmsg::strt || (char)send_crc == gxx::gmsg::stub) {
							u->sendbyte(gxx::gmsg::stub);
							send_state = 4;
							return;
						}

						send_state = 2;
						u->sendbyte(send_crc);
						return;
					}

					c = *send_ptr++;
					strmcrc8(&send_crc, c);

					if (c==gxx::gmsg::strt || c==gxx::gmsg::stub) {
						u->sendbyte(gxx::gmsg::stub);
						send_state = 1;
					} else {
						u->sendbyte(c);
					}
					return;
				case 1:
					switch(*(send_ptr-1)) {
						case gxx::gmsg::strt: u->sendbyte(gxx::gmsg::stub_strt); break;
						case gxx::gmsg::stub: u->sendbyte(gxx::gmsg::stub_stub); break;
					}
					send_state = 0;
					return;
				case 2:
					u->sendbyte(gxx::gmsg::strt);
					send_state = 3;
					return;
				case 3:
					u->enable_tx_irq(false);
					sended();
					return;
				case 4:
					switch((char)send_crc) {
						case gxx::gmsg::strt: u->sendbyte(gxx::gmsg::stub_strt); break;
						case gxx::gmsg::stub: u->sendbyte(gxx::gmsg::stub_stub); break;
					}
					send_state = 2;
					return;
			}
		}

		void sended() {
			auto tmp = insend;

			if (to_send.empty()) {
				insend = nullptr;
			}
			else {
				do_send(&*to_send.begin());
			}

			g1::return_to_tower(tmp, g1::status::Sended);
		}
	};
}

#endif