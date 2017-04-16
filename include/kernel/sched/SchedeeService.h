#include <kernel/service/MsgTag.h>
#include <kernel/service/MessageManager.h>
#include <kernel/service/Service.h>
#include <datastruct/hlist_head.h>
#include <kernel/id/id.h>
#include <kernel/csection.h>
#include <kernel/panic.h>

namespace Genos {
	/*class SchedeeService : public Service, public Schedee {
		MessageHeader * messageSlot;
		//qid_t waitFrom;
		MessageList queries;

	private:
		int8_t send() {
			assert(messageSlot);
			
			Glue::systemSchedeeManager().waitSchedee(*this, SCHEDEE_BLOCKED_SEND);
			
			auto ret = transportQuery(messageSlot);
			if (ret != 0) {
				Glue::systemSchedeeManager().runSchedee(*this);
				return -1;
			}

			return 0;
		}
		
		void receive() {
			if (!queries.empty()) {

			}
		}
		
		void reply() {

		}

	public:
		MessageHeader* prepareMessage() {
			auto header = Genos::Glue::allocateMessageHeader();
			messageSlot = header;
			return header;
		}

		MessageHeader* getMessageSlot() {
			return messageSlot;
		}

		int8_t receiveQuery(MessageHeader *header) {
			if (state == SCHEDEE_BLOCKED_RECEIVE) {
				messageSlot = header;
				Glue::systemSchedeeManager().runSchedee(*this);
			} else {
				queries.move_back(*header);		
			}
		}

		int8_t receiveAnswer(MessageHeader *header) {
			if (state == SCHEDEE_BLOCKED_SEND && messageSlot == header) {
				Glue::systemSchedeeManager().runSchedee(*this);				
			} else {
				panic("wrong answer");
			}
			
		}

		~SchedeeService() {
			if (messageSlot && messageSlot->state.status == MessageHeader::Status::Near) {
				Genos::Glue::releaseMessageHeader(messageSlot);
			}
		}	
	};*/
}