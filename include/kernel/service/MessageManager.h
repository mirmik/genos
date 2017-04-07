#ifndef GENOS_MESSAGE_MANAGER_H
#define GENOS_MESSAGE_MANAGER_H

#include <debug/dprint.h>

#include <kernel/csection.h>
#include <kernel/service/MsgTag.h>
#include <kernel/service/ServiceTable.h>
#include <gxx/pool.h>
#include <gxx/dlist.h>
#include <gxx/array.h>

#include <kernel/sched/AutomSchedee.h>

namespace Genos {
	
	class MessageManagerBasic {
	protected:
		MessageList queries;
		MessageList replies;

		gxx::block_pool<MsgTag, 10> msgTagPool;
		gxx::block_pool<ipcstack, 10> ipcStackPool;

	public:

		MessageManagerBasic() {};

		MsgTag* getAnonimMsgTag() {
			//auto stack = ipcStackPool.emplace((new gxx::array<stack_item, 5>)->slice());
			auto msgtag = msgTagPool.emplace();
			return msgtag;
		}

		ipcstack* getIpcStack() {
			auto stack = ipcStackPool.emplace((new gxx::array<stack_item, 5>)->slice());
			return stack;
		}

	protected:
		void send(MsgTag& msgtag) {
			dprln("MessageManager::send");
			
			Service* receiver = Glue::getService(msgtag.receiver);
			assert(receiver);
			
			receiver->receiveMessage(msgtag);
		}

		void reply(MsgTag& msgtag) {
			dprln("MessageManager::reply");
		}
	};

	class MessageManager : public MessageManagerBasic {
	public:

		void toSend(MsgTag& msgtag) {
			critical_section_enter();
			queries.move_back(msgtag);
			critical_section_leave();
		}

		void toReply(MsgTag& msgtag) {
			critical_section_enter();
			replies.move_back(msgtag);
			critical_section_leave();
		}

		void execute() {
			while (!queries.empty()) {
				MsgTag & msg = *queries.begin();
				dlist_del_init(&msg.lnk);
				send(msg);
			}

			while (!replies.empty()) {
				MsgTag & msg = *replies.begin();
				dlist_del_init(&msg.lnk);
				reply(msg);
			}			
		}

	};
}

#endif