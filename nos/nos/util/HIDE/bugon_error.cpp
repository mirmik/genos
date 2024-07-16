#include <nos/bugon.h>
#include <stdlib.h>

#include <nos/util/error.h>

void bugon_impl(const char* str) {
	nos::error(str);
}
