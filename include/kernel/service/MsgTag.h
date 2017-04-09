#ifndef GENOS_KERNEL_QUERY_H
#define GENOS_KERNEL_QUERY_H

#include <kernel/ipcstack/ipcstack.h>
#include <kernel/ipcstack/MessageStack.h>
#include <kernel/id/id.h>
#include <gxx/dlist.h>
#include <gxx/ref_ptr.h>

#define MSGTAG_ANSWER 0x01
#define MSGTAG_KERNEL_STACK 0x02

namespace Genos {

	namespace Glue {
		MessageStack* allocateMessageStack(); 
		void releaseMessageStack(MessageStack*);
	}

	class MessageStackDeleter {
	public:
		void deleter(MessageStack* ptr){
			Glue::releaseMessageStack(ptr);
		}
	};

	class MessageHeader {
	public:
		struct dlist_head lnk;
		uint8_t ref = 0;

		gxx::ref_ptr<MessageStack, &MessageStack::ref, MessageStackDeleter> stack;

		qid_t sender;
		qid_t receiver;

		MessageHeader() {
			dlist_init(&lnk);
		}

		//void bind(ipcstack* stack) {
		//	this->stack = stack;
		//}
	};

	namespace Glue {
		MessageHeader* allocateMessageHeader(); 
		void releaseMessageHeader(MessageHeader*);
	}

	using MessageList = gxx::dlist<MessageHeader, &MessageHeader::lnk>;

	class MessageHeaderDeleter {
	public:
		void deleter(MessageHeader* ptr){
			Glue::releaseMessageHeader(ptr);
		}
	};

	class MsgBody {
	public:
		gxx::ref_ptr<MessageStack, &MessageStack::ref, MessageStackDeleter> stack;

		MsgBody() : stack(Glue::allocateMessageStack()) {} 
	};

	class MsgTag {
	public:
		gxx::ref_ptr<MessageHeader, &MessageHeader::ref, MessageHeaderDeleter> header;

		MsgTag() : header(Glue::allocateMessageHeader()) {}
		MsgTag(MessageHeader* head) : header(head) {}

		MsgTag(const MsgTag& other) : header(other.header) {}

		void bind(MsgBody& body) {
			header->stack.reset(body.stack);
		}
	};

	namespace Glue {
		void replyMessage(MsgTag& msgtag);
	}
}

#endif