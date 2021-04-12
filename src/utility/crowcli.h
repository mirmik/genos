#ifndef GENOS_CROW_CROWCLI_H
#define GENOS_CROW_CROWCLI_H

#include <genos/sched.h>
#include <genos/schedee.h>
#include <crow/proto/node.h>

#include <igris/defs/schedee.h>
#include <igris/sync/syslock.h>
#include <igris/shell/executor.h>

#define CROWCLI_OTPUT_BUFFER_SIZE 256

namespace genos
{
	class crowcli : public genos::schedee, public crow::node
	{
	public:
		dlist_head msgqueue = DLIST_HEAD_INIT(msgqueue);
		dlist_head waitlink = DLIST_HEAD_INIT(waitlink);
		igris::executor * exec;

		int state = 0;
		char output_buffer[CROWCLI_OTPUT_BUFFER_SIZE];
		genos::buffer_node output;

		void set_executor(igris::executor * exec) { this->exec = exec; }

		crowcli() : output(output_buffer, CROWCLI_OTPUT_BUFFER_SIZE) 
		{
			schedee_init(this, 0);
			flag.can_displace = 1;
		}

		void finalize() override { BUG(); }
		int displace() override
		{
			flag.runned = 0;
			__context_displace_vector__();
		}

		void execute() override
		{
			switch (state)
			{
				case 0:
					genos::open_resource(&output, 0);
					genos::open_resource(&output, 0);
					state = 1;
					break;
				case 1:
					{
						system_lock();

						if (dlist_empty(&msgqueue))
						{
							system_unlock();
							wait_current_schedee(&waitlink, 0, nullptr);
							return;
						}

						output.reset();
						crow::packet * cptr = 
							dlist_first_entry(&msgqueue, crow::packet, ulnk);
						dlist_del_init(&cptr->ulnk);

						system_unlock();
						
						auto data = crow::node_data(cptr);
						auto rid = crow::node_protocol_cls::sid(cptr);

						if (data.data()[data.size()-1]=='\n')
							data.data()[data.size()-1] = 0;

						memset(output_buffer, 0, CROWCLI_OTPUT_BUFFER_SIZE);
						int ret1;


						int ret0 = exec->execute(data.data(), data.size(), 
							SH_INTERNAL_SPLIT, &ret1);
						(void) ret0;

						auto outputbuf = output.buffer();

						crow::node_send(id, rid, cptr->addr(), outputbuf, 
							cptr->qos(), cptr->ackquant());

						crow::release(cptr);
						break;
					}
			}
		}

		void incoming_packet(crow::packet * cptr) override
		{
			system_lock();
			dlist_add_tail(&cptr->ulnk, &msgqueue);
			system_unlock();
			unwait_one(&waitlink, nullptr);
		}

		void undelivered_packet(crow::packet * cptr) override
		{
			crow::release(cptr);
		}
	};
}

#endif