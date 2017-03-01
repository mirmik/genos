#include <kernel/ipcstack/ipcstack.h>

void debug_stack_item_dump(struct stack_item* it) {
	switch(it->type) {
		case ItemType::Bool: debug_print(it->i32 ? "true" : "false"); break;
		case ItemType::Nil: break;
		case ItemType::Int32: debug_printdec_int32(it->i32); break;
		case ItemType::Float: debug_print("TODO"); break;
		case ItemType::String: debug_write(it->str->data, it->str->size); break;
		case ItemType::Vector: debug_stack_vector_dump(it->vec); break;
	}
}

void stack_release_item(struct stack_item* it) {
	switch (it->type) {
		case ItemType::String: 		release_string_item(it->str);  break;
		case ItemType::Vector: 		release_vector_item(it->vec);  break;
		case ItemType::Dictionary: 	release_dictionary_item(it->dct);  break;
	}
}