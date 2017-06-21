#ifndef GENOS_MESSAGE_STACK_H
#define GENOS_MESSAGE_STACK_H

#include <inttypes.h>
#include <debug/dprint.h>

#include <gxx/array.h>
#include <kernel/ipcstack/ipcstack.h>
#include <kernel/devices/serial/Stream.h>

namespace Genos {
	class MessageStack {
	public:
		uint8_t ref;
		gxx::array<stack_item, 8> stack_buffer;
		ipcstack stack;

		MessageStack() : stack(stack_buffer.slice()) {}

		void push_nil() {
			stack_push_nil(&stack);
		}

		void push_int32(int32_t num) {
			stack_push_int32(&stack, num);
		}

		void pop(int i = 1) {
			stack_pop(&stack, i);
		}

		void dump_item(Stream& strm, struct stack_item* it) {
			switch (it->type) {
				case ItemType_Nil:	
					strm.print("Nil");
					break;
				case ItemType_Int32:
					strm.print("Int32:");
					strm.print((int32_t)it->i32);	
					break;
				default: 
					strm.print("UnresolvedType");
					break;
			}
			
		}

		void dump(Stream& strm) {
			strm.print("ipcstack_dump. total: "); 
			strm.print(stack.total); 
			strm.print(", errstat: "); 
			strm.println(stack.errstat);
			for (int i = 0; i < stack.total; i++) {
				struct stack_item* it = stack.buffer + i;
				dump_item(strm, it);
				strm.println();
			}
		}
	};
}

#endif 