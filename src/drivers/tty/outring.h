#ifndef GENOS_OUTRING_H
#define GENOS_OUTRING_H

namespace genos 
{
	class outring 
	{
		ring_head ring;
		char * buf;

		genos::uart * uart;


	};
}

#endif