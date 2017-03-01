#ifndef GENOS_IPCSTACK_H
#define GENOS_IPCSTACK_H

#include <kernel/ipcstack/string.h>
#include <kernel/ipcstack/vector.h>
#include <kernel/ipcstack/dictionary.h>

#include <kernel/ipcstack/item.h>

#include <assert.h>
#include <compiler.h>

struct ipcstack {
public:
	//data
	stack_item* buffer;
	size_t 		size;
	int total = 0;

	ipcstack(gxx::slice<stack_item> slc) : buffer(slc.data()), size(slc.size()) {} 
};

__BEGIN_DECLS

void stack_push_nil(struct ipcstack* stack);
void stack_push_bool(struct ipcstack* stack, int8_t val);
void stack_push_int32(struct ipcstack* stack, int32_t val);
void stack_push_float(struct ipcstack* stack, float val);

void stack_push_string(struct ipcstack* stack, const char* data, size_t size);
char* stack_push_empty_string(struct ipcstack* stack, size_t size);
void stack_push_local_string(struct ipcstack* stack, const char* data, size_t size);

void stack_push_vector(struct ipcstack* stack);

struct stack_item* stack_get_item(struct ipcstack* stack, int index);

int32_t stack_get_bool(struct ipcstack* stack, int index, int* isbool = NULL);
int32_t stack_get_int32(struct ipcstack* stack, int index, int* isi32 = NULL);
int32_t stack_get_string_size(struct ipcstack* stack, int index, int* isstr = NULL);
int32_t stack_get_string(struct ipcstack* stack, int index, char* buffer, size_t bufsize,  int* isstr = NULL);

void stack_item_addref(stack_item* it);

int32_t stack_vector_append(struct ipcstack* stack, int32_t vecindex, int32_t itemindex);

void stack_pop(struct ipcstack* stack, int count);
void stack_pop_all(struct ipcstack* stack);

const char* stack_item_type_2_str(uint8_t type);
void debug_ipcstack_dump(struct ipcstack* stack);

__END_DECLS

#endif