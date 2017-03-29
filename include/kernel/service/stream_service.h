#ifndef GENOS_MULTI_HANDLER_SERVICE_H
#define GENOS_MULTI_HANDLER_SERVICE_H

#include <hal/irq.h>

#include <gxx/dlist.h>
#include <kernel/service/service.h>

#include <proto/stream.h>

namespace Genos {

	class StreamService : public Service {
	public:
		enum {
			WriteTag,
			ReadTag,
			FlushTag,
			AvailTag,
			IrqWriteFinishTag,
			IrqReadFinishTag,
		};

		gxx::dlist<MsgTag, &MsgTag::lnk> postbox;

		MsgTag* 	current_write_message;
		gxx::buffer write_buffer;
		size_t 		write_cursor;

		MsgTag* 	current_read_message;
		gxx::buffer read_buffer;
		size_t 		read_cursor;

		int8_t receiveMessage(MsgTag& msg) {
			__label__ __fast_reply__, __exit__, __irq_exit__, __write_find_iter__;

			gxx::dlist<MsgTag, &MsgTag::lnk>::iterator next;
			ipcstack* stack = msg.stack;

			int8_t type = stack_get_int32(stack, 0, NULL);

			debug_ipcstack_dump(stack);

			auto save = global_irq_save();

			switch (type) {
				case WriteTag: 
					dprln("StreamService::WriteTag");
					if (current_write_message == nullptr) writeInit(msg);
					else postbox.move_back(msg);
					goto __exit__;	
				
				case ReadTag: 
					dprln("StreamService::ReadTag");
					//if (read_message == nullptr) restype = readInit();
					//else postbox.move_back(msg);
					goto __exit__;
				
				case FlushTag: 
					dprln("StreamService::FlushTag");
					if (current_write_message == nullptr) goto __fast_reply__;
					else postbox.move_back(msg);
					goto __exit__;	
				
				case AvailTag: 
					dprln("StreamService::AvailTag");
					stack_pop_all(stack);
					stack_push_int32(stack, avail());
					goto __fast_reply__;	

				case IrqWriteFinishTag:
					dprln("StreamService::IrqWriteFinishTag"); 
					ReplyMessage(*current_write_message);

					__write_find_iter__:
					next = gxx::find_if(postbox.begin(), postbox.end(), [](const MsgTag& tag){
						int32_t type = stack_get_int32(tag.stack, 0, nullptr);
						return type == WriteTag || type == FlushTag;
					});
					
					if (next == postbox.end()) goto __irq_exit__;
					
					dlist_del_init(&next->lnk);

					switch (type) {
						case FlushTag: 
							ReplyMessage(*next);
							goto __write_find_iter__;
						case WriteTag:
							if (writeInit(*next)) goto __irq_exit__;
							else goto __write_find_iter__;
					}

				case IrqReadFinishTag: 
					dprln("StreamService::IrqReadFinishTag");
					
					goto __fast_reply__;	
			}

			__irq_exit__:
			__fast_reply__:
			ReplyMessage(msg);

			__exit__:
			global_irq_restore(save);
		}

	public:
		
		int8_t writeInit(MsgTag& msg) {

		};

		virtual int8_t readInit() {};
		virtual int8_t avail() { return 0; };

		virtual int8_t writeStart() {};
	};

}

#endif