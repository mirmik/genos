#ifndef GENOS_SOCKET_H
#define GENOS_SOCKET_H

#include <igris/datastruct/ring.h>

#include <genos/wait.h>
#include <genos/resource.h>
#include <crow/proto/channel.h>
#include <crow/tower.h>

namespace genos
{
	class socket : public node
	{
		virtual int disconnect();
		virtual int connect();
		virtual int listen(int max);
		virtual int bind(int port);
	};

	class crow_socket : public socket
	{
		char rxbuf[32];
		ring_head rxring = RING_HEAD_INIT(32);
		struct dlist_head rxwait = DLIST_HEAD_INIT(rxwait);


	public:
		crow::channel ch;

	public:
		int bind(int chid) override
		{
			crow::link_channel(&ch, chid);
			return chid;
		}

		int listen(int max = 0) override
		{
			(void)max;
			ch.wait_handshake_request();
			return 0;
		}

		static void handler(crow::channel* self, crow::packet* pack)
		{
			crow_socket * sock = mcast_out(self, crow_socket, ch);

			igris::buffer data = crow::channel::getdata(pack);
			ring_write(&sock->rxring, sock->rxbuf, data.data(), data.size());

			unwait_one(&sock->rxwait, 0);

			crow::release(pack);
		}

		crow_socket() :
			ch(handler)
		{
			dlist_init(&rxwait);
		}

		int connect() override { return 0;}
		int disconnect() override { return 0; }

		ssize_t read(void* data, size_t size, int flags) 
		{
			return waited_ring_read(data, size, flags,
				&rxring, rxbuf, &rxwait);
		}
	};
}

#endif