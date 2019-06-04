#ifndef GENOS_SOCKET_H
#define GENOS_SOCKET_H

#include <igris/compiler.h>
#include <genos/>

namespace genos 
{
	class socket : public node
	{
		virtual int disconnect();
		virtual int connect();
		virtual int listen();
		virtual int bind();
	};
}

#endif