#ifndef GENOS_MESSAGE_BOX_H
#define GENOS_MESSAGE_BOX_H

#include <crow/node.h>
namespace genos {
	struct message {
		uint16_t sid;
		uint16_t rid;
		void* data
	};
}

namespace genos {
	struct schedee_msgbox : public crow::node {
		schedee* sch;

		gxx::dlist<crow::packet, &crow::packet::lnk> inlist;

		void*  raddrptr;
		size_t raddrlen;

		void Query();
		void Send();
		void Receive();
		void Reply();
		void Release();
	};
}

#endif