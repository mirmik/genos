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

	public:

		MessageManagerBasic() {};

		//MessageHeader* getAnonimMessageHeader() {
			//auto stack = ipcStackPool.emplace((new gxx::array<stack_item, 5>)->slice());
		//	auto msgtag = msgTagPool.emplace();
		//	return msgtag;
		//}

		//ipcstack* getIpcStack() {
		//	auto stack = ipcStackPool.emplace((new gxx::array<stack_item, 5>)->slice());
		//	return stack;
		//}

	protected:
		void send(MessageHeader& header) {
			dprln("MessageManager::send");
			
			Service* receiver = Glue::getService(header.receiver);
			assert(receiver);
			
			receiver->receiveMessageHandler(Genos::MsgTag(&header));
		}

		void reply(MessageHeader& msgtag) {
			dprln("MessageManager::reply");
		}
	};

	class MessageManager : public MessageManagerBasic {
	public:

		void toSend(MessageHeader& msgtag) {
			atomic_section_enter();
			queries.move_back(msgtag);
			atomic_section_leave();
		}

		void toReply(MessageHeader& msgtag) {
			atomic_section_enter();
			replies.move_back(msgtag);
			atomic_section_leave();
		}

		void toReplyMsgTag(MsgTag& tag) {
			toReply(*tag.header);
		}

		void execute() {
			while (!queries.empty()) {
				MessageHeader & msg = *queries.begin();
				dlist_del_init(&msg.lnk);
				send(msg);
			}

			while (!replies.empty()) {
				MessageHeader & msg = *replies.begin();
				dlist_del_init(&msg.lnk);
				reply(msg);
			}			
		}

	};
}

#endif