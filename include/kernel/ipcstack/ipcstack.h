#ifndef GENOS_IPCSTACK_H
#define GENOS_IPCSTACK_H

#include <kernel/ipcstack/string.h>
#include <kernel/ipcstack/vector.h>
#include <kernel/ipcstack/dictionary.h>

#include <kernel/ipcstack/item.h>

#include <assert.h>

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

static void stack_push_vector(struct ipcstack* stack) {
	assert(stack->total != stack->size);
	stack_item* it = stack->buffer + stack->total++;
	it->type = ItemType::Vector;

	struct vector_item* vec = construct_vector_item();

	it->vec = vec;
}

static char* stack_push_empty_string(struct ipcstack* stack, size_t size) {
	assert(stack->total != stack->size);
	stack_item* it = stack->buffer + stack->total++;
	it->type = ItemType::String;

	struct string_item* str = construct_empty_string_item(size);

	it->str = str;
	return str->data;
}

static stack_item* stack_get_item(struct ipcstack* stack, int index) {
	index = index < 0 ? index + stack->total : index;
	assert(index >= 0);
	assert(index < stack->total);
	return stack->buffer + index;
}

static int32_t stack_get_bool(struct ipcstack* stack, int index, int* isbool = NULL) {
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

static int32_t stack_get_int32(struct ipcstack* stack, int index, int* isi32 = NULL) {
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

static int32_t stack_get_string_size(struct ipcstack* stack, int index, int* isstr = NULL) {
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

static int32_t stack_get_string(struct ipcstack* stack, int index, char* buffer, size_t bufsize,  int* isstr = NULL) {
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

static void stack_item_addref(stack_item* it) {
	switch (it->type) {
		case ItemType::String: 		it->str->ref++;  break;
		case ItemType::Vector: 		it->vec->ref++;  break;
		case ItemType::Dictionary: 	it->dct->ref++;  break;
	}
}

void stack_release_item(stack_item* it);

static int32_t stack_vector_append(struct ipcstack* stack, int32_t vecindex, int32_t itemindex) {
	stack_item* vecit = stack_get_item(stack, vecindex);
	stack_item* itit = stack_get_item(stack, itemindex);
	if (vecit->type == ItemType::Vector) {
		stack_item_addref(itit);
		vector_item_append(vecit->vec, itit);
	}
	else {
		return -1;
	}	
}

static void stack_pop(struct ipcstack* stack, int count) {
	assert(stack->total >= count);
	while(count--) {
		stack_item* it = stack->buffer + (stack->total - 1); 
		
		stack_release_item(it);

		stack->total--;
	}
}

static void stack_pop_all(struct ipcstack* stack) {
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

void debug_stack_item_dump(struct stack_item* it);
static void debug_ipcstack_dump(struct ipcstack* stack) {
	debug_print("ipcstack_dump. total: "); debug_printdec_int32(stack->total); dln();
	for (int i = 0; i < stack->total; i++) {
		stack_item* it = stack->buffer + i;
		debug_print(stack_item_type_2_str(it->type)); debug_print(": ");
		debug_stack_item_dump(it);
		dln();
	}
}

#endif