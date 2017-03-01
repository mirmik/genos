#include <kernel/ipcstack/vector.h>

vector_item* construct_vector_item() {
	struct vector_item* vec = new vector_item; 
	vec->ref = 1;
	return vec;
}

void release_vector_item(struct vector_item* vec) {
	if( ! --vec->ref ) {
		auto end = vec->vec.end();
		for(auto it = vec->vec.begin(); it!=end; it++) {
			stack_release_item(it);
		}

		delete vec;
	}
}

void vector_item_append(struct vector_item* vec, struct stack_item* it) {
	vec->vec.push_back(*it);
}

void debug_stack_vector_dump(struct vector_item* vec) {
	debug_print("[");
	auto end = vec->vec.end();
	for(auto it = vec->vec.begin(); it != end; it++) {
		debug_stack_item_dump(&*it); 
		if(it + 1 != end) debug_print(",");
	}
	debug_print("]");
}