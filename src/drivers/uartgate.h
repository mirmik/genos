#ifndef G1_UARTGATE_H
#define G1_UARTGATE_H

#include <g1/gateway.h>
#include <gxx/io/iostream.h>

namespace g1 {
	struct uartgate : public g1::gateway {
		gxx::dlist<packet, &packet::lnk> to_send;
		char* senditer;
		size_t sendsize;

		void send(g1::packet* pack) {
			to_send.move_back(pack);
		}

		void nonblock_onestep() override {
			debug_print("onestep_execute");
		}
	};
}

#endif