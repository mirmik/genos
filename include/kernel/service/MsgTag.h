#ifndef GENOS_KERNEL_QUERY_H
#define GENOS_KERNEL_QUERY_H

#include <kernel/ipcstack/ipcstack.h>
#include <kernel/id/id.h>
#include <gxx/dlist.h>

#define MSGTAG_ANSWER 0x01
#define MSGTAG_KERNEL_STACK 0x02

namespace Genos {

	class MsgTag {
	public:
		struct dlist_head lnk;
		
		ipcstack * stack;
		int8_t stat;
	
		qid_t sender;
		qid_t receiver;

		MsgTag() {
			dlist_init(&lnk);
		}

		void bind(ipcstack* stack) {
			this->stack = stack;
		}
	}; 
	
	using MessageList = gxx::dlist<MsgTag, &MsgTag::lnk>;

	namespace Glue {
		void replyMessage(MsgTag& msgtag);
	}
	void ReplyMessage(MsgTag& msgtag) { Glue::replyMessage(msgtag); }

}

#endif