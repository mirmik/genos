#ifndef GENOS_KERNEL_QUERY_H
#define GENOS_KERNEL_QUERY_H

#include <kernel/ipcstack/ipcstack.h>
#include <kernel/ipcstack/MessageStack.h>
#include <kernel/id/id.h>
#include <gxx/dlist.h>
#include <gxx/ref_ptr.h>

namespace Genos {

	/*class MessageStackDeleter {
	public:
		void deleter(MessageStack* ptr){
			Glue::releaseMessageStack(ptr);
		}
	};*/

	class MessageHeader {
	public:
		enum Status {
			Near = 0,
			Away = 1,
		};

		struct State {
			bool noanswer : 1;
			Status status : 2;
			State() : noanswer(0), status(Status::Near) {}
		};

		struct dlist_head lnk;
		//uint8_t ref = 0;

		//gxx::ref_ptr<MessageStack, &MessageStack::ref, MessageStackDeleter> stack;
		MessageStack* stack;
		State state;

		qid_t sender = 0;
		qid_t receiver = 0;

		MessageHeader() {
			dlist_init(&lnk);
		}

		//void bind(ipcstack* stack) {
		//	this->stack = stack;
		//}
	};

	using MessageList = gxx::dlist<MessageHeader, &MessageHeader::lnk>;

	/*struct MessageHeaderState {
		uint8_t noanswer : 1;
	};

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
		MessageHeaderState state;

		qid_t sender;
		qid_t receiver;

		MessageHeader() {
			dlist_init(&lnk);
		}

		//void bind(ipcstack* stack) {
		//	this->stack = stack;
		//}
	};
*/
	namespace Glue {
		MessageHeader* allocateMessageHeader(); 
		void releaseMessageHeader(MessageHeader*);
	}

/*
	class MessageHeaderDeleter {
	public:
		void deleter(MessageHeader* ptr){
			Glue::releaseMessageHeader(ptr);
		}
	};

	class MsgBody {
	public:
		gxx::ref_ptr<MessageStack, &MessageStack::ref, MessageStackDeleter> stack;

		MsgBody(){}
		MsgBody(MessageStack* stack) : stack(stack) {};
		MsgBody(const MsgBody& other) : stack(other.stack) {};

		MessageStack* operator->() {
			return stack.get();
		}

		void detach() {
			stack.reset(nullptr);			
		}
	};

	class MsgTag {
	public:
		gxx::ref_ptr<MessageHeader, &MessageHeader::ref, MessageHeaderDeleter> header;

		MsgTag(){}
		MsgTag(MessageHeader* head) : header(head) {}

		MsgTag(const MsgTag& other) : header(other.header) {}

		void bind(MsgBody& body) {
			header->stack.reset(body.stack);
		}

		MsgBody get() {
			return MsgBody(header->stack.get());
		}

		void detach() {
			header.reset(nullptr);			
		}

		MessageHeader* operator->() {
			return header.get();
		}
	};

	namespace Glue {
		void replyMessage(MsgTag& msgtag);
	}*/
}

static void dump(const Genos::MessageHeader& header) {
	dprln("MessageHeader:");
	dpr("\tsender: "); dprln(header.sender);
	dpr("\treceiver: "); dprln(header.receiver);
	dpr("\tnoanswer: "); dprln(header.state.noanswer);
	dpr("\tstatus: "); dprln((int)header.state.status);
}

#endif