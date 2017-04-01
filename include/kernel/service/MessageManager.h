#ifndef GENOS_MESSAGE_MANAGER_H
#define GENOS_MESSAGE_MANAGER_H

#include <debug/dprint.h>

#include <kernel/csection.h>
#include <kernel/service/MsgTag.h>
#include <kernel/service/ServiceTable.h>
#include <gxx/dlist.h>

namespace Genos {
	
	class MessageManager {
		ServiceTable* serviceTable;

		MessageList queries;
		MessageList replies;

	public:

		MessageManager(ServiceTable* table) :
			serviceTable(table) {};

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
			dprln("MessageManager::execute");
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

	private:
		void send(MsgTag& msgtag) {
			dprln("MessageManager::send");
		}

		void reply(MsgTag& msgtag) {
			dprln("MessageManager::reply");
		}
	};
}

#endif