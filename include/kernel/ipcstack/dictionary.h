#ifndef GENOS_KERNEL_ITEM_dictionary_H
#define GENOS_KERNEL_ITEM_dictionary_H

#include <kernel/ipcstack/item.h>

struct dictionary_item {
	uint8_t ref;
};

static dictionary_item* construct_dictionary_item() {
	struct dictionary_item* dct = new dictionary_item; 
	dct->ref = 1;
	return dct;
}

static void release_dictionary_item(struct dictionary_item* dct) {
	if( ! --dct->ref ) {
		delete dct;
	}
}

#endif