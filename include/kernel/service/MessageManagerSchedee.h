#ifndef GENOS_MESSAGE_MANAGER_SCHEDEE_H
#define GENOS_MESSAGE_MANAGER_SCHEDEE_H

#include <kernel/service/MessageManager.h>

namespace Genos {
	
	class MessageManagerSchedee : public MessageManagerBasic, public AutomFunctorSchedee {
	public:

		void toSend(MessageHeader& msgtag) {
			critical_section_enter();
			queries.move_back(msgtag);
			run();
			critical_section_leave();
		}

		void toReply(MessageHeader& msgtag) {
			critical_section_enter();
			replies.move_back(msgtag);
			run();
			critical_section_leave();
		}

		void toReplyMsgTag(MsgTag& tag) {
			toReply(*tag.header);
		}

		void routine() override {
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

			stop();			
		}

	};
}

#endif