#include <kernel/ipcstack/ipcstack.h>


void stack_push_nil(struct ipcstack* stack) {
	assert(stack->total != stack->size);
	struct stack_item* it = stack->buffer + stack->total++;
	it->type = ItemType_Nil;
}

void stack_push_bool(struct ipcstack* stack, int8_t val) {
	assert(stack->total != stack->size);
	struct stack_item* it = stack->buffer + stack->total++;
	it->type = ItemType_Bool;
	it->i32 = val;
}

void stack_push_int32(struct ipcstack* stack, int32_t val) {
	assert(stack->total != stack->size);
	struct stack_item* it = stack->buffer + stack->total++;
	it->type = ItemType_Int32;
	it->i32 = val;
}

void stack_push_float(struct ipcstack* stack, float val) {
	assert(stack->total != stack->size);
	struct stack_item* it = stack->buffer + stack->total++;
	it->type = ItemType_Float;
	it->flt = val;
}

void stack_push_bytestring(struct ipcstack* stack, const char* data, size_t size) {
	assert(stack->total != stack->size);
	struct stack_item* it = stack->buffer + stack->total++;
	it->type = ItemType_String;

	struct string_item* str = construct_string_item(data, size);

	it->str = str;
}

void stack_push_string(struct ipcstack* stack, const char* data) {
	stack_push_bytestring(stack, data, strlen(data));
}

char* stack_push_empty_string(struct ipcstack* stack, size_t size) {
	assert(stack->total != stack->size);
	struct stack_item* it = stack->buffer + stack->total++;
	it->type = ItemType_String;

	struct string_item* str = construct_empty_string_item(size);

	it->str = str;
	return str->data;
}

void stack_push_local_string(struct ipcstack* stack, const char* data, size_t size) {
	assert(stack->total != stack->size);
	struct stack_item* it = stack->buffer + stack->total++;
	it->type = ItemType_String;

	struct string_item* str = construct_local_string_item(data, size);

	it->str = str;
}

void stack_push_vector(struct ipcstack* stack) {
	assert(stack->total != stack->size);
	struct stack_item* it = stack->buffer + stack->total++;
	it->type = ItemType_Vector;

	struct vector_item* vec = construct_vector_item();

	it->vec = vec;
}

struct stack_item* stack_get_item(struct ipcstack* stack, int index) {
	index = index < 0 ? index + stack->total : index;
	assert(index >= 0);
	assert(index < stack->total);
	return stack->buffer + index;
}

int32_t stack_get_bool(struct ipcstack* stack, int index, int* isbool) {
	struct stack_item* it = stack_get_item(stack, index);
	if (it->type == ItemType_Bool) {
		if (isbool) *isbool = 1; 
		return it->i32 != 0;
	}
	else {
		if (isbool) *isbool = 0; 
		return 0;
	}
}

int32_t stack_get_int32(struct ipcstack* stack, int index, int* isi32) {
	struct stack_item* it = stack_get_item(stack, index);
	if (it->type == ItemType_Int32) {
		if (isi32) *isi32 = 1; 
		return it->i32;
	}
	else {
		if (isi32) *isi32 = 0; 
		return 0;
	}
}

int32_t stack_get_string_size(struct ipcstack* stack, int index, int* isstr) {
	struct stack_item* it = stack_get_item(stack, index);
	if (it->type == ItemType_String) {
		if (isstr) *isstr = 1; 
		return it->str->size;
	}
	else {
		if (isstr) *isstr = 0; 
		return 0;
	}
}

int32_t stack_get_string(struct ipcstack* stack, int index, char* buffer, size_t bufsize,  int* isstr) {
	struct stack_item* it = stack_get_item(stack, index);
	if (it->type == ItemType_String) {
		size_t csize = it->str->size < bufsize ? it->str->size : bufsize;
		memcpy(buffer, it->str->data, csize);
		return csize;
	}
	else {
		if (isstr) *isstr = 0; 
		return 0;
	}	
}

void stack_item_addref(struct stack_item* it) {
	switch (it->type) {
		case ItemType_String: 		it->str->ref++;  break;
		case ItemType_Vector: 		it->vec->ref++;  break;
		case ItemType_Dictionary: 	it->dct->ref++;  break;
	}
}


int32_t stack_vector_append(struct ipcstack* stack, int32_t vecindex, int32_t itemindex) {
	struct stack_item* vecit = stack_get_item(stack, vecindex);
	struct stack_item* itit = stack_get_item(stack, itemindex);
	if (vecit->type == ItemType_Vector) {
		stack_item_addref(itit);
		vector_item_append(vecit->vec, itit);
		return 0;
	}
	else {
		return -1;
	}	
}

void stack_pop(struct ipcstack* stack, int count) {
	assert(stack->total >= count);
	while(count--) {
		struct stack_item* it = stack->buffer + (stack->total - 1); 
		
		stack_release_item(it);

		stack->total--;
	}
}

void stack_pop_all(struct ipcstack* stack) {
	stack_pop(stack, stack->total);
}

void stack_set_errstate(struct ipcstack* stack, int err) {
	stack->errstat = err;
}

void stack_set_error(struct ipcstack* stack, int errcode, const char* message) {
	stack_set_errstate(stack, errcode);
	stack_pop_all(stack);
	stack_push_local_string(stack, message, strlen(message));
}

const char* stack_item_type_2_str(uint8_t type) {
	switch (type) {
		case ItemType_Nil:				return "Nil";
		case ItemType_Int32:			return "Int32";
		case ItemType_Float:			return "Float";
		case ItemType_Bool:			return "Bool";
		case ItemType_Vector:			return "Vector";
		case ItemType_String:			return "String";
		case ItemType_Dictionary:		return "Dictionary";
		default: 						return "UnresolvedType";
	}
}

void debug_ipcstack_dump(struct ipcstack* stack) {
	debug_print("ipcstack_dump. total: "); debug_printdec_int32(stack->total); 
	debug_print(", errstat: "); debug_printdec_int32(stack->errstat); dln();
	for (int i = 0; i < stack->total; i++) {
		struct stack_item* it = stack->buffer + i;
		debug_print(stack_item_type_2_str(it->type)); debug_print(": ");
		debug_stack_item_dump(it);
		dln();
	}
}

int stack_is_error(struct ipcstack* stack) {
	return stack->errstat;
}

const char* stack_error_message(struct ipcstack* stack) {
	return stack_get_item(stack, -1)->str->data;
}
