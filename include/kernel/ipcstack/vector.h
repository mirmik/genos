#ifndef GENOS_KERNEL_ITEM_VECTOR_H
#define GENOS_KERNEL_ITEM_VECTOR_H

#include <gxx/Vector.h>
#include <kernel/ipcstack/item.h>

void debug_stack_item_dump(struct stack_item* it);
void stack_release_item(struct stack_item* it);

struct vector_item {
	uint8_t ref;
	gxx::Vector<stack_item> vec;
};

__BEGIN_DECLS

vector_item* construct_vector_item();

void release_vector_item(struct vector_item* vec);

void vector_item_append(struct vector_item* vec, struct stack_item* it);

void debug_stack_vector_dump(struct vector_item* vec);

__END_DECLS

#endif