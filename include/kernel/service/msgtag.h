#ifndef GENOS_KERNEL_QUERY_H
#define GENOS_KERNEL_QUERY_H

#include <kernel/ipcstack/ipcstack.h>
#include <kernel/id/id.h>
#include <datastruct/dlist_head.h>

#define MSGTAG_ANSWER 0x01
#define MSGTAG_KERNEL_STACK 0x02

namespace Genos {

	struct MsgTag {
		struct dlist_head lnk;
		
		ipcstack * stack;
		int8_t stat;
	
		qid_t sender;
		qid_t receiver;

		//MsgTag(size_t stcksize) {}
	}; 
	
	static MsgTag* msgtag_allocate() {
		return new MsgTag;
	}
	
	static MsgTag* msgtag_release(MsgTag* msg) {
		//if (msg->stat & MSGTAG_KERNEL_STACK) release_ipcstack();
		delete msg;
	}
	
	static void ReplyMessage(MsgTag& msg) {
		if (msg.stat & MSGTAG_ANSWER) {
	
		} else {
			msgtag_release(&msg);
		}
	}

}

#endif