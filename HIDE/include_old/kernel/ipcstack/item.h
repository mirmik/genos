#ifndef GENOS_KERNEL_ITEM_H
#define GENOS_KERNEL_ITEM_H

struct string_item;
struct vector_item;
struct dictionary_item;

#include <inttypes.h>
#include <genos/compiler.h>

enum ItemType {
	//BasicTypes
	ItemType_Int32 = 0,
	ItemType_Float = 1,
	ItemType_Nil = 2,
	ItemType_Bool = 3,
	ItemType_String = 4,
	
	//Collection
	ItemType_Vector = 5,
	ItemType_Dictionary = 6,
};

struct stack_item {
	uint8_t type;
	union {
		int32_t 			i32;
		float 				flt;
		struct string_item* 		str;
		struct vector_item* 		vec;
		struct dictionary_item* 	dct;
	};	
};

__BEGIN_DECLS

void stack_release_item(struct stack_item* it);
void debug_stack_item_dump(struct stack_item* it);

__END_DECLS

#endif