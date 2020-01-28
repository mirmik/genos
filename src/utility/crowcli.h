#ifndef GENOS_CROW_CROWCLI_H
#define GENOS_CROW_CROWCLI_H

#include <genos/schedee.h>
#include <crow/proto/node.h>

#include <igris/defs/schedee.h>
#include <igris/sync/syslock.h>

namespace genos 
{
	class crowcli : public genos::schedee, public crow::node
	{
	public:
		dlist_head msgqueue;
		igris::executor * exec;

		int state = 0;
		char output_buffer[128];
		genos::buffer_resource output;

		crowcli() : output(output_buffer, 128) {} 

		void finalize() override { BUG(); }
		int displace() override { return SCHEDEE_DISPLACE_ERROR; }

		void execute() override 
		{
			switch (state) 
			{
				case 0:
					genos::opennode(&output);
					genos::opennode(&output);
					state = 1;
					break;
				case 1:
					break;
			}
		}

		void incoming_packet(crow::packet * cptr) override 
		{ 
			igris::syslock syslock;
			dlist_add_tail(&cptr->ulnk, &msgqueue);
		}
		
		void undelivered_packet(crow::packet * cptr) override 
		{
			crow::release(cptr);
		}
	};
}

#endif