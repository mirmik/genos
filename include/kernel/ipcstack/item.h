#ifndef GENOS_KERNEL_ITEM_H
#define GENOS_KERNEL_ITEM_H

struct string_item;
struct vector_item;
struct dictionary_item;

#include <inttypes.h>
#include <compiler.h>

enum ItemType {
	//BasicTypes
	Int32 = 0,
	Float = 1,
	Nil = 2,
	Bool = 3,
	String = 4,
	
	//Collection
	Vector = 5,
	Dictionary = 6,
};

struct stack_item {
	uint8_t type;
	union {
		int32_t 			i32;
		float 				flt;
		string_item* 		str;
		vector_item* 		vec;
		dictionary_item* 	dct;
	};	
};

__BEGIN_DECLS

void stack_release_item(struct stack_item* it);
void debug_stack_item_dump(struct stack_item* it);

__END_DECLS

#endif