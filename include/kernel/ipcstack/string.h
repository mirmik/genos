#ifndef GENOS_KERNEL_ITEM_STRING_H
#define GENOS_KERNEL_ITEM_STRING_H

#include <debug/dprint.h>
#include <kernel/ipcstack/item.h>

#include <string.h>
#include <stdlib.h>

#include <compiler.h>

#define LOCAL_STRING_ITEM 0x01

struct string_item {
	uint8_t ref;
	char* data;
	size_t size;
	uint8_t flag;
};

__BEGIN_DECLS

string_item* construct_string_item(const char* data, size_t size);
string_item* construct_local_string_item(const char* data, size_t size);
string_item* construct_empty_string_item(size_t size);
void release_string_item(struct string_item* str);

__END_DECLS

#endif