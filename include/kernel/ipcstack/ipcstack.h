#ifndef GENOS_IPCSTACK_H
#define GENOS_IPCSTACK_H

#include <gxx/slice.h>
#include <gxx/ByteArray.h>
#include <gxx/vector.h>
#include <genos/sigslot/delegate.h>
#include <gxx/DList.h>

enum MsgItemType{
	Nil = 0,
	Buffer = 1,
	AllocatedBuffer = 2,
	Word = 3,
	Int32 = 4,
	
	String = 5,
	Vector = 6,
};


class ipcstack {
public:
	class item {
	public:
		uint8_t type;
		union {
			int32_t 			i32;
			uintptr_t 			uptr;
			gxx::slice<char> 	buf;
			gxx::string 		str;
			gxx::vector<item> 	vec; 
		};

		item(item&& other) {
			memcpy(this, &other, sizeof(item));
		}

		item& operator=(item&& other) {
			memcpy(this, &other, sizeof(item));
			return *this;
		}

		item(){};
		~item(){};
	};

	//data
	gxx::slice<item> m_stack;
	int total = 0;

	ipcstack(gxx::slice<item> slc) : m_stack(slc) {} 

	item& get_item(int index) {
		index = index < 0 ? index + total : index;
		assert(index >= 0);
		assert(index < total);
		return m_stack[index];
	}

	int32_t get_int32(int index) {
		item& it = get_item(index);
		assert(it.type == MsgItemType::Int32);
		return it.i32;
	}

	uintptr_t get_word(int index) {
		item& it = get_item(index);
		assert(it.type == MsgItemType::Word);
		return it.uptr;
	}

	gxx::buffer get_buffer(int index) {
		item& it = get_item(index);
		assert(it.type == MsgItemType::Buffer || it.type == MsgItemType::AllocatedBuffer);
		return it.buf;
	}

	gxx::string& get_string(int index) {
		item& it = get_item(index);
		assert(it.type == MsgItemType::String);
		return it.str;
	}

	gxx::vector<item>& get_vector(int index) {
		item& it = get_item(index);
		assert(it.type == MsgItemType::Vector);
		return it.vec;
	}

	void push_nil() {
		assert(total != m_stack.size());
		item& it = m_stack[total++];
		it.type = MsgItemType::Nil;
	}

	void push_word(uintptr_t word) {
		assert(total != m_stack.size());
		item& it = m_stack[total++];
		it.type = MsgItemType::Word;
		it.uptr = word;
	}

	void push_int32(int32_t val) {
		assert(total != m_stack.size());
		item& it = m_stack[total++];
		it.type = MsgItemType::Int32;
		it.i32 = val;
	}

	void push_buffer(char* data, size_t size) {
		assert(total != m_stack.size());
		item& it = m_stack[total++];
		it.type = MsgItemType::Buffer;
		it.buf = gxx::slice<char>(data, size);		
	}

	void push_buffer(gxx::buffer buf) {
		push_buffer(buf.data(), buf.size());		
	}

	void push_allocated_buffer(const char* data, size_t size) {
		assert(total != m_stack.size());
		item& it = m_stack[total++];
		it.type = MsgItemType::AllocatedBuffer;
		char* __data = strndup(data, size);
		it.buf = gxx::slice<char>(__data, size);				
	}

	void push_string() {
		assert(total != m_stack.size());
		item& it = m_stack[total++];
		it.type = MsgItemType::String;
		new (&it.str) gxx::string();		
	}

	void push_vector() {
		assert(total != m_stack.size());
		item& it = m_stack[total++];
		it.type = MsgItemType::Vector;
		new (&it.vec) gxx::vector<item>();		
	}

	void vector_append(int count) {
		gxx::vector<item>& vec = get_vector(- count - 1);
		vec.reserve(vec.size() + count);
		for (int i = -1; i > - count - 1; i--) {
			vec.emplace_back(gxx::move(get_item(i)));
		}
		pop(count);
	}

	void pop(int count) {
		assert(total >= count);
		while(count--) {
			item& it = m_stack[total - 1]; 
			
			switch (it.type) {
				case MsgItemType::Word:
				case MsgItemType::Nil:
				case MsgItemType::Int32:
				case MsgItemType::Buffer: 
					it.type = MsgItemType::Nil; 
				break;

				case MsgItemType::AllocatedBuffer: 
					free(it.buf.data()); 
					it.type = MsgItemType::Nil; 
				break;

				case MsgItemType::String: 
					it.str.~ByteArray();
					it.type = MsgItemType::Nil; 
				break;
			}
			total--;
		}
	}

	void pop_all() {
		pop(total);
	}

	const char* item_type_2_str(uint8_t type) {
		switch (type) {
			case MsgItemType::Nil:				return "Nil";
			case MsgItemType::Buffer:			return "Buffer";
			case MsgItemType::Word:				return "Word";
			case MsgItemType::Int32:			return "Int32";
			case MsgItemType::Vector:			return "Vector";
			default: 							return "UnresolvedType";
		}
	}

	void debug_stack_dump() {
		debug_print("ipcstack_dump. total: "); debug_printdec_int32(total); dln();
		for (int i = 0; i < total; i++) {
			auto& it = m_stack[i];
			debug_print(item_type_2_str(it.type)); debug_print(": ");
			switch(m_stack[i].type) {
				case MsgItemType::Buffer: debug_write(it.buf.data(), it.buf.size()); break;
				case MsgItemType::Nil: break;
				case MsgItemType::Word: debug_printhex_ptr((void*)it.uptr); break;
				case MsgItemType::Int32: debug_printdec_int32(it.i32); break;
				case MsgItemType::Vector: debug_printdec_int32(it.vec.size()); break;
			}
			dln();
		}
	}
};

class ipcservice {
public:
	virtual void execute(ipcstack& stack, delegate<void> callback) = 0;
};

#endif