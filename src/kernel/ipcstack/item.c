#include <kernel/ipcstack/ipcstack.h>
#include <kernel/ipcstack/item.h>

void debug_stack_item_dump(struct stack_item* it) {
	switch(it->type) {
		case ItemType_Bool: debug_print(it->i32 ? "true" : "false"); break;
		case ItemType_Nil: break;
		case ItemType_Int32: debug_printdec_int32(it->i32); break;
		case ItemType_Float: debug_print("TODO"); break;
		case ItemType_String: debug_write(it->str->data, it->str->size); break;
		case ItemType_Vector: debug_stack_vector_dump(it->vec); break;
	}
}

void stack_release_item(struct stack_item* it) {
	switch (it->type) {
		case ItemType_String: 		release_string_item(it->str);  break;
		case ItemType_Vector: 		release_vector_item(it->vec);  break;
		case ItemType_Dictionary: 	release_dictionary_item(it->dct);  break;
	}
}