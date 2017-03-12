#ifndef GENOS_KERNEL_ITEM_dictionary_H
#define GENOS_KERNEL_ITEM_dictionary_H

#include <kernel/ipcstack/item.h>

struct dictionary_item {
	uint8_t ref;
};

__BEGIN_DECLS

struct dictionary_item* construct_dictionary_item();

void release_dictionary_item(struct dictionary_item* dct);

__END_DECLS

#endif