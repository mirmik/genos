#ifndef GENOS_IPCSTACK_H
#define GENOS_IPCSTACK_H

#include <kernel/ipcstack/string.h>
#include <kernel/ipcstack/vector.h>
#include <kernel/ipcstack/dictionary.h>

#include <kernel/ipcstack/item.h>

struct ipcstack {
public:
	//data
	stack_item* buffer;
	size_t 		size;
	int total = 0;

	ipcstack(gxx::slice<stack_item> slc) : buffer(slc.data()), size(slc.size()) {} 
};

static void stack_push_nil(struct ipcstack* stack) {
	assert(stack->total != stack->size);
	stack_item* it = stack->buffer + stack->total++;
	it->type = ItemType::Nil;
}

static void stack_push_bool(struct ipcstack* stack, int8_t val) {
	assert(stack->total != stack->size);
	stack_item* it = stack->buffer + stack->total++;
	it->type = ItemType::Bool;
	it->i32 = val;
}

static void stack_push_int32(struct ipcstack* stack, int32_t val) {
	assert(stack->total != stack->size);
	stack_item* it = stack->buffer + stack->total++;
	it->type = ItemType::Int32;
	it->i32 = val;
}

static void stack_push_float(struct ipcstack* stack, float val) {
	assert(stack->total != stack->size);
	stack_item* it = stack->buffer + stack->total++;
	it->type = ItemType::Float;
	it->flt = val;
}

static void stack_push_string(struct ipcstack* stack, const char* data, size_t size) {
	assert(stack->total != stack->size);
	stack_item* it = stack->buffer + stack->total++;
	it->type = ItemType::String;

	struct string_item* str = construct_string_item(data, size);

	it->str = str;
}

static char* stack_push_empty_string(struct ipcstack* stack, size_t size) {
	assert(stack->total != stack->size);
	stack_item* it = stack->buffer + stack->total++;
	it->type = ItemType::String;

	struct string_item* str = construct_empty_string_item(size);

	it->str = str;
	return str->data;
}


stack_item* stack_get_item(struct ipcstack* stack, int index) {
	index = index < 0 ? index + stack->total : index;
	assert(index >= 0);
	assert(index < stack->total);
	return stack->buffer + index;
}

int32_t stack_get_bool(struct ipcstack* stack, int index, int* isbool = NULL) {
	stack_item* it = stack_get_item(stack, index);
	if (it->type == ItemType::Bool) {
		if (isbool) *isbool = 1; 
		return it->i32 != 0;
	}
	else {
		if (isbool) *isbool = 0; 
		return 0;
	}
}

int32_t stack_get_int32(struct ipcstack* stack, int index, int* isi32 = NULL) {
	stack_item* it = stack_get_item(stack, index);
	if (it->type == ItemType::Int32) {
		if (isi32) *isi32 = 1; 
		return it->i32;
	}
	else {
		if (isi32) *isi32 = 0; 
		return 0;
	}
}

int32_t stack_get_string_size(struct ipcstack* stack, int index, int* isstr = NULL) {
	stack_item* it = stack_get_item(stack, index);
	if (it->type == ItemType::String) {
		if (isstr) *isstr = 1; 
		return it->str->size;
	}
	else {
		if (isstr) *isstr = 0; 
		return 0;
	}
}

int32_t stack_get_string(struct ipcstack* stack, int index, char* buffer, size_t bufsize,  int* isstr = NULL) {
	stack_item* it = stack_get_item(stack, index);
	if (it->type == ItemType::String) {
		size_t csize = it->str->size < bufsize ? it->str->size : bufsize;
		memcpy(buffer, it->str->data, csize);
		return csize;
	}
	else {
		if (isstr) *isstr = 0; 
		return 0;
	}
	
}

	/*gxx::string& get_string(int index) {
		item& it = get_item(index);
		assert(it.type == MsgItemType::String);
		return it.str;
	}

	gxx::vector<item>& get_vector(int index) {
		item& it = get_item(index);
		assert(it.type == MsgItemType::Vector);
		return it.vec;
	}*/

/*
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

	/*void push_string() {
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
	}*/

void stack_pop(struct ipcstack* stack, int count) {
	assert(stack->total >= count);
	while(count--) {
		stack_item* it = stack->buffer + (stack->total - 1); 
		
		switch (it->type) {
			case ItemType::String: release_string_item(it->str);  break;
		}

		stack->total--;
	}
}

void stack_pop_all(struct ipcstack* stack) {
	stack_pop(stack, stack->total);
}


static const char* stack_item_type_2_str(uint8_t type) {
	switch (type) {
		case ItemType::Nil:				return "Nil";
		case ItemType::Int32:			return "Int32";
		case ItemType::Float:			return "Float";
		case ItemType::Bool:			return "Bool";
		case ItemType::Vector:			return "Vector";
		case ItemType::String:			return "String";
		case ItemType::Dictionary:		return "Dictionary";
		default: 						return "UnresolvedType";
	}
}

static void debug_ipcstack_dump(struct ipcstack* stack) {
	debug_print("ipcstack_dump. total: "); debug_printdec_int32(stack->total); dln();
	for (int i = 0; i < stack->total; i++) {
		stack_item* it = stack->buffer + i;
		debug_print(stack_item_type_2_str(it->type)); debug_print(": ");
		switch(it->type) {
			//case ItemType::: debug_write(it.buf.data(), it.buf.size()); break;
			case ItemType::Bool: debug_print(it->i32 ? "true" : "false"); break;
			case ItemType::Nil: break;
			case ItemType::Int32: debug_printdec_int32(it->i32); break;
			case ItemType::Float: debug_print("TODO"); break;
			case ItemType::String: debug_write(it->str->data, it->str->size); break;
		}
		dln();
	}
}
/*
class ipcservice {
public:
	virtual void execute(ipcstack& stack, delegate<void> callback) = 0;
};*/

#endif