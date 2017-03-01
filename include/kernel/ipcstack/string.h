#ifndef GENOS_KERNEL_ITEM_STRING_H
#define GENOS_KERNEL_ITEM_STRING_H

#include <kernel/ipcstack/item.h>

#include <string.h>
#include <stdlib.h>

struct string_item {
	uint8_t ref;
	char* data;
	size_t size;
};

static string_item* construct_string_item(const char* data, size_t size) {
	struct string_item* str = (struct string_item*) malloc(sizeof(struct string_item)); 
	str->data = (char*) malloc(size);
	str->size = size;
	str->ref = 1;
	memcpy(str->data, data, size);
	return str;
}

static string_item* construct_empty_string_item(size_t size) {
	struct string_item* str = (struct string_item*) malloc(sizeof(struct string_item)); 
	str->data = (char*) malloc(size);
	str->size = size;
	str->ref = 1;
	return str;
}

static void release_string_item(struct string_item* str) {
	if( ! --str->ref ) {
		debug_print("FREE_STRING_ITEM");dln();
		free(str->data);
		free(str);
	}
}

#endif