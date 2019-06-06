#ifndef GENOS_SOCKET_H
#define GENOS_SOCKET_H

#include <genos/resource.h>
#include <crow/channel.h>

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
		char buf[32];
		ring_head rxring;

		crow::channel ch;

		int bind(int chid) override 
		{
			crow::link_channel(&ch);
		}	

		int listen(int max = 0) override 
		{
			(void)max;

			ch->wait_handshake();
		}
	};
}

#endif