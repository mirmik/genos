#include <kernel/ipcstack/string.h>

string_item* construct_string_item(const char* data, size_t size) {
	struct string_item* str = (struct string_item*) malloc(sizeof(struct string_item)); 
	str->data = (char*) malloc(size);
	str->size = size;
	str->ref = 1;
	str->flag = 0;
	memcpy(str->data, data, size);
	return str;
}

string_item* construct_local_string_item(const char* data, size_t size) {
	struct string_item* str = (struct string_item*) malloc(sizeof(struct string_item)); 
	str->data = (char*) data;
	str->size = size;
	str->ref = 1;
	str->flag = LOCAL_STRING_ITEM;
	return str;
}

string_item* construct_empty_string_item(size_t size) {
	struct string_item* str = (struct string_item*) malloc(sizeof(struct string_item)); 
	str->data = (char*) malloc(size);
	str->size = size;
	str->ref = 1;
	str->flag = 0;
	return str;
}

void release_string_item(struct string_item* str) {
	if( ! --str->ref ) {
		debug_print("FREE_STRING_ITEM");dln();
		if (! (str->flag & LOCAL_STRING_ITEM)) free(str->data);
		free(str);
	}
}